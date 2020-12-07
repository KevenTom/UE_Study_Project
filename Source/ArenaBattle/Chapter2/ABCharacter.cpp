// Fill out your copyright notice in the Description page of Project Settings.


#include "ABCharacter.h"

// Sets default values
AABCharacter::AABCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));

	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_REVENANT(TEXT("/Game/ParagonRevenant/Characters/Heroes/Revenant/Meshes/Revenant.Revenant"));
	if (SK_REVENANT.Succeeded()) {
		GetMesh()->SetSkeletalMesh(SK_REVENANT.Object);
		GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	}
	static ConstructorHelpers::FClassFinder<UAnimInstance> REVENANT_ANIM(TEXT("/Game/ParagonRevenant/Characters/Heroes/Revenant/Animations/AnimBP_Revenant.AnimBP_Revenant_C"));
	if (REVENANT_ANIM.Succeeded()) {
		GetMesh()->SetAnimInstanceClass(REVENANT_ANIM.Class);
	}

	SetControlMode(0);

	fire = CreateDefaultSubobject<UParticleSystem>(TEXT("FIREE"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> PS_FIRE(TEXT("/Game/AdvancedMagicFX12/particles/P_ky_hit_fire.P_ky_hit_fire"));
	if (PS_FIRE.Succeeded()) {
		fire = PS_FIRE.Object;
	}
}

// Called when the game starts or when spawned
void AABCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AABCharacter::SetControlMode(int32 ControlMode) {
	if (ControlMode == 0) {
		SpringArm->TargetArmLength = 450.f;
		SpringArm->SetRelativeRotation(FRotator::ZeroRotator);
		SpringArm->bUsePawnControlRotation = true;
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
	}
}

// Called every frame
void AABCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AABCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AABCharacter::UpDown);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AABCharacter::LeftRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AABCharacter::LookUpDown);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &AABCharacter::LookLeftRight);
	PlayerInputComponent->BindAction(TEXT("LightClock"), IE_Pressed, this, &AABCharacter::Attack);
}

void AABCharacter::UpDown(float AxisValue) {
	AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::X), AxisValue);
}

void AABCharacter::LeftRight(float AxisValue) {
	AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::Y), AxisValue);
}

void AABCharacter::LookUpDown(float AxisValue) {
	AddControllerPitchInput(-AxisValue);
}

void AABCharacter::LookLeftRight(float AxisValue) {
	AddControllerYawInput(AxisValue);
}

void AABCharacter::Attack() {
	for (float i = 0; i < 10; ++i) {
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), fire, FTransform(FVector(i * 50, 0, 0)), true);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), fire, FTransform(FVector(i * 50, i * 50, 0)), true);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), fire, FTransform(FVector(0, i * 50, 0)), true);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), fire, FTransform(FVector(-(i * 50), i * 50, 0)), true);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), fire, FTransform(FVector(-(i * 50), 0, 0)), true);
	}
}
