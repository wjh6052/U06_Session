#include "FPSCharacter.h"
#include "Global.h"

#include "Animation/AnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "Particles/ParticleSystemComponent.h"

#include "Actors/CBullet.h"
#include "Game/CGameState.h"


AFPSCharacter::AFPSCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(44.f, 88.0f);


	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(GetCapsuleComponent());
	Camera->SetRelativeLocation(FVector(0, 0, 64.f));
	Camera->bUsePawnControlRotation = true;

	ConstructorHelpers::FObjectFinder<USkeletalMesh> tp_MeshAsset(TEXT("SkeletalMesh'/Game/AnimStarterPack/UE4_Mannequin/Mesh/SK_Mannequin.SK_Mannequin'"));
	if (tp_MeshAsset.Succeeded())
		GetMesh()->SetSkeletalMesh(tp_MeshAsset.Object);

	ConstructorHelpers::FClassFinder<UAnimInstance> tp_AnimClass(TEXT("/Game/AnimStarterPack/UE4ASP_HeroTPP_AnimBlueprint"));
	if (tp_AnimClass.Succeeded())
		GetMesh()->SetAnimInstanceClass(tp_AnimClass.Class);

	GetMesh()->SetOwnerNoSee(true);
	GetMesh()->SetRelativeLocation(FVector(0, 0, -88));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));


	FP_Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterFP_Mesh"));
	FP_Mesh->SetOnlyOwnerSee(true);
	FP_Mesh->SetupAttachment(Camera);
	FP_Mesh->bCastDynamicShadow = false;
	FP_Mesh->CastShadow = false;
	FP_Mesh->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));
	FP_Mesh->SetRelativeRotation(FRotator(1.9f, -19.2f, 5.2f));

	ConstructorHelpers::FObjectFinder<USkeletalMesh> fp_MeshAsset(TEXT("/Game/FirstPerson/Character/Mesh/SK_Mannequin_Arms"));
	if (fp_MeshAsset.Succeeded())
		FP_Mesh->SetSkeletalMesh(fp_MeshAsset.Object);

	ConstructorHelpers::FClassFinder<UAnimInstance> fp_AnimClass(TEXT("/Game/FirstPerson/Animations/FirstPerson_AnimBP"));
	if (fp_AnimClass.Succeeded())
		FP_Mesh->SetAnimInstanceClass(fp_AnimClass.Class);


	FP_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	FP_Gun->SetOnlyOwnerSee(true);
	FP_Gun->bCastDynamicShadow = false;
	FP_Gun->CastShadow = false;
	FP_Gun->SetupAttachment(FP_Mesh, TEXT("GripPoint"));


	TP_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("TP_Gun"));
	TP_Gun->SetOwnerNoSee(true);
	TP_Gun->SetupAttachment(GetMesh(), "hand_r");
	TP_Gun->SetRelativeLocation(FVector(-9.8f, 5, 0));
	TP_Gun->SetRelativeRotation(FRotator(0, 90, 0));


	ConstructorHelpers::FObjectFinder<USkeletalMesh> gunAsset(TEXT("/Game/FirstPerson/FPWeapon/Mesh/SK_FPGun"));
	if (gunAsset.Succeeded())
	{
		TP_Gun->SetSkeletalMesh(gunAsset.Object);
		FP_Gun->SetSkeletalMesh(gunAsset.Object);
	}


	CHelpers::CreateSceneComponent(this, &FP_GunShotParticle, "FP_GunShotParticle", FP_Gun);
	FP_GunShotParticle->SetupAttachment(FP_Gun, "Muzzle");
	FP_GunShotParticle->bAutoActivate = false;
	FP_GunShotParticle->SetOnlyOwnerSee(true);


	CHelpers::CreateSceneComponent(this, &TP_GunShotParticle, "TP_GunShotParticle", TP_Gun);
	TP_GunShotParticle->SetupAttachment(TP_Gun, "Muzzle");
	TP_GunShotParticle->bAutoActivate = false;
	TP_GunShotParticle->SetOwnerNoSee(true);

}


void AFPSCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);
	
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AFPSCharacter::OnFire);
	
	PlayerInputComponent->BindAxis("MoveForward", this, &AFPSCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFPSCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AFPSCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AFPSCharacter::LookUpAtRate);
}



void AFPSCharacter::BeginPlay()
{
	Super::BeginPlay();

	ACGameState* gameState = Cast<ACGameState>(GetWorld()->GetGameState());
	if (!!gameState)
	{
		CLog::Print(gameState->TestTeam == ETeamType::BlueTeam ? "Blud" : "Red");
		CLog::Print(gameState->PlayerArray.Num());
	}
	else
	{
		CLog::Print("GameState is not found");
	}
}

void AFPSCharacter::OnFire()
{
	if (!!FP_FireAnimation)
	{
		UAnimInstance* animInstance = FP_Mesh->GetAnimInstance();
		if (!!animInstance)
		{
			animInstance->Montage_Play(FP_FireAnimation, 1.f);
		}
	}

	if (!!FP_GunShotParticle)
		FP_GunShotParticle->Activate(true);


	APlayerController* playerController = Cast<APlayerController>(GetController());
	
	FVector shootDir = FVector::ZeroVector;
	FVector startTrace = FVector::ZeroVector;

	if (playerController)
	{
		FRotator camRot;
		playerController->GetPlayerViewPoint(startTrace, camRot);
		shootDir = camRot.Vector();

		startTrace = startTrace + shootDir * ((GetActorLocation() - startTrace) | shootDir);
	}

	const FVector endTrace = startTrace + shootDir * WeaponRange;
	
	const FHitResult impact = WeaponTrace(startTrace, endTrace);

	AActor* damagedActor = impact.GetActor();
	UPrimitiveComponent* damagedComponent = impact.GetComponent();

	if ((!!damagedActor) && (damagedActor != this) && (!!damagedComponent) && damagedComponent->IsSimulatingPhysics())
	{
		damagedComponent->AddImpulseAtLocation(shootDir * WeaponDamage, impact.Location);
	}

	OnServerFire(startTrace, endTrace);
}


void AFPSCharacter::OnServerFire_Implementation(const FVector& InLineStart, const FVector& InLineEnd)
{
	NetMulticast_ShootEffects();
}


void AFPSCharacter::NetMulticast_ShootEffects_Implementation()
{
	if (!!TP_FireAnimation)
	{
		UAnimInstance* animInstance = GetMesh()->GetAnimInstance();
		if (!!animInstance)
		{
			animInstance->Montage_Play(TP_FireAnimation, 1.f);
		}
	}


	if (!!FireSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}


	if (!!TP_GunShotParticle)
		TP_GunShotParticle->Activate(true);

	if (!!BulletClass)
		GetWorld()->SpawnActor<ACBullet>(BulletClass, FP_Gun->GetSocketLocation("Muzzle"), FP_Gun->GetSocketRotation("Muzzle"));
}


void AFPSCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AFPSCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AFPSCharacter::TurnAtRate(float Rate)
{
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AFPSCharacter::LookUpAtRate(float Rate)
{
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

FHitResult AFPSCharacter::WeaponTrace(const FVector& StartTrace, const FVector& EndTrace) const
{
	FCollisionQueryParams TraceParams(SCENE_QUERY_STAT(WeaponTrace), true, GetInstigator());
	TraceParams.bReturnPhysicalMaterial = true;

	FHitResult Hit(ForceInit);
	GetWorld()->LineTraceSingleByChannel(Hit, StartTrace, EndTrace, ECC_GameTraceChannel1, TraceParams);

	return Hit;
}
