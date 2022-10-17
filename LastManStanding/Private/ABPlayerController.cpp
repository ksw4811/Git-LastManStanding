// Fill out your copyright notice in the Description page of Project Settings.


#include "ABPlayerController.h"
#include "GameMain_HUD.h"
#include "ABAnimInstance.h"
#include "ABCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "ABGameInstance.h"
#include "Kismet/KismetMathLibrary.h"

AABPlayerController::AABPlayerController()
{
	//fSprintSpeedMultiPlayer = 3.0f; // ó���� 3.0, �̼Ǽ���� 2.5 2.0 1.5 �ܰ�� �پ�� 
}

void AABPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AABPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
	myPawn = aPawn;
	myCharacter = Cast<AABCharacter>(myPawn);
	myCharacter->MyCharacterDead.AddUObject(this, &AABPlayerController::PlayerKill); // �� ĳ���Ͱ� �׾��ٴ� ��������Ʈ �Լ��� �߻��ϸ�?

	PlayerEnter();
}

void AABPlayerController::PlayerEnter()
{
	CtoS_PlayerEnter();
}

void AABPlayerController::CtoS_PlayerEnter_Implementation()
{
	TArray<AActor*> OutActors;
	int PlayerCount = 0;
	UGameplayStatics::GetAllActorsOfClass(GetPawn()->GetWorld(), APlayerController::StaticClass(), OutActors);
	for (AActor* OutActor : OutActors)
	{
		AABPlayerController* PC = Cast<AABPlayerController>(OutActor);
		if (PC)
		{
			++PlayerCount;
		}
	}
	myCharacter->nNowPlayer = PlayerCount;
	for (AActor* OutActor : OutActors)
	{
		AABPlayerController* PC = Cast<AABPlayerController>(OutActor);
		if (PC)
		{
			PC->StoC_PlayerEnter(myCharacter->nNowPlayer);
		}
	}
}

void AABPlayerController::StoC_PlayerEnter_Implementation(int PlayerCount)
{
	if (myCharacter == NULL) // ĳ���Ϳ� ���ǵ��� ���� ��쿡�� �������� �ʰ�����.
	{
		return;
	}
	myCharacter->nNowPlayer = PlayerCount;
	FString testPlayerCount = FString::FromInt(myCharacter->nNowPlayer);
	//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, testPlayerCount);
}

void AABPlayerController::PlayerKill()
{
	CtoS_PlayerKill(); // Ŭ���̾�Ʈ���� ȣ���ϰ� �������� �����ϴ� �Լ�
}

void AABPlayerController::CtoS_PlayerKill_Implementation()
{
	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(GetPawn()->GetWorld(), APlayerController::StaticClass(), OutActors);
	myCharacter->nPlayerKill++;
	for (AActor* OutActor : OutActors)
	{
		AABPlayerController* PC = Cast<AABPlayerController>(OutActor);
		if (PC)
		{
			PC->StoC_PlayerKill(myCharacter->nPlayerKill); // ���������� ��� Ŭ���̾�Ʈ�鿡�� �� �Լ��� �����϶���Ѵ�.
		}
	}
}

void AABPlayerController::StoC_PlayerKill_Implementation(int nPlayerKill)
{
	if (myCharacter == NULL) // ĳ���Ϳ� ���ǵ��� ���� ��쿡�� �������� �ʰ�����.
	{
		return;
	}
	myCharacter->nPlayerKill = nPlayerKill;
	//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, testPlayerCount);
}





void AABPlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetShowMouseCursor(false);
	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);
}

void AABPlayerController::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	// ���⼭ Killing Point ����
	SetPlayerKillingPoint(myCharacter->nKillingCharacter);

	if (myCharacter->nKillingCharacter == 10 || myCharacter->nPlayerKill == myCharacter->nNowPlayer - 1)
	{
		UABGameInstance* MyGI = GetGameInstance<UABGameInstance>(); // GameInstance�� ���� ���� ���
		FString UserName = MyGI->GetUserName("Player");

		GameEnd(UserName);
	}
}


void AABPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	//AABCharacter* myCharacter = Cast<AABCharacter>(myPawn);

	// Ű ���ε�

	InputComponent->BindAction(TEXT("Chat"), EInputEvent::IE_Pressed, this, &AABPlayerController::FocusChatInputText);

	// ������

	//InputComponent->BindAxis(TEXT("UpDown"), this, &AABPlayerController::UpDown);
	//InputComponent->BindAxis(TEXT("LeftRight"), this, &AABPlayerController::LeftRight);
	//InputComponent->BindAxis(TEXT("LookUp"), this, &AABPlayerController::LookUp);
	//InputComponent->BindAxis(TEXT("Turn"), this, &AABPlayerController::Turn);

	// �׼�

	InputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &AABPlayerController::Jump);
	InputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Released, this, &AABPlayerController::CheckMission);
	InputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &AABPlayerController::Attack);
	InputComponent->BindAction(TEXT("Run"), EInputEvent::IE_Pressed, this, &AABPlayerController::Run);
	InputComponent->BindAction(TEXT("Run"), EInputEvent::IE_Released, this, &AABPlayerController::StopRun);
}

// ������ �Լ�
/*
void AABPlayerController::UpDown(float NewAxisValue)
{
	APawn* const myPawn = GetPawn();
	AABCharacter* myCharacter = Cast<AABCharacter>(myPawn);
	//myPawn->AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::X), NewAxisValue);
}

void AABPlayerController::LeftRight(float NewAxisValue)
{
	APawn* const myPawn = GetPawn();
	AABCharacter* myCharacter = Cast<AABCharacter>(myPawn);
	//myPawn->AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::Y), NewAxisValue);
}


void AABPlayerController::LookUp(float NewAxisValue)
{
	APawn* const myPawn = GetPawn();
	//AABCharacter* myCharacter = Cast<AABCharacter>(myPawn);
	//myPawn->AddControllerPitchInput(NewAxisValue);
}

void AABPlayerController::Turn(float NewAxisValue)
{
	APawn* const myPawn = GetPawn();
	//AABCharacter* myCharacter = Cast<AABCharacter>(myPawn);
	//myPawn->AddControllerYawInput(NewAxisValue);
}
*/
// �׼� �Լ�


void AABPlayerController::Jump()
{
	//APawn* const myPawn = GetPawn();
	//AABCharacter* myCharacter = Cast<AABCharacter>(myPawn);
	//ABCharacter = Cast <AABCharacter>(ABPawn);
	if (myCharacter->CurrentState == ECharacterState::READY)
	{
		myCharacter->bPressedJump = true;
		myCharacter->JumpKeyHoldTime = 0.0f;
	}
}

void AABPlayerController::CheckMission()
{
	if (bMissionClear == false && myCharacter->nMissionClear == 1) // ĳ���� üũ�ϴ°� �ڷεּ� bool �ٸ��� ���ϰԲ�
	{
		SetPlayerMissionClear(myCharacter->nMissionClear);

		// ���⼭ �޸��� ���ϰ� �ϰ� �ӵ� ����
		bCanRun = false;
		bMissionClear = true; // �ٽ� ����ȵǰԲ�
	}
}

// �޸���
void AABPlayerController::Run()
{
	//APawn* const myPawn = GetPawn();
	//AABCharacter* myCharacter = Cast<AABCharacter>(myPawn);

	//myCharacter->GetCharacterMovement()->MaxWalkSpeed *= myCharacter->fSprintSpeedMultiPlayer;

	if (myCharacter->CurrentState == ECharacterState::READY && bCanRun == true)
	{
		if (myCharacter == nullptr) return;
		//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("PlayerRun!"));

		CtoS_Run(myCharacter);
		//MyRun.Broadcast();
	}
}

void AABPlayerController::CtoS_Run_Implementation(AABCharacter* ClientCharacter)
{
	// ���������� ��� PlayerController���� �̺�Ʈ�� ������.
	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(GetPawn()->GetWorld(), APlayerController::StaticClass(), OutActors);
	for (AActor* OutActor : OutActors)
	{
		AABPlayerController* PC = Cast<AABPlayerController>(OutActor);
		if (PC)
		{
			PC->StoC_Run(ClientCharacter);
		}
	}
}

void AABPlayerController::StoC_Run_Implementation(AABCharacter* ClientCharacter)
{
	// ������ Ŭ���̾�Ʈ�� �� �̺�Ʈ�� �޾Ƽ� �����Ѵ�.

	if (ClientCharacter == nullptr) return;

	if (bCanRun == false)
	{
		myCharacter->GetCharacterMovement()->MaxWalkSpeed = 200.0f;
	}

	ClientCharacter->GetCharacterMovement()->MaxWalkSpeed *= ClientCharacter->fSprintSpeedMultiPlayer;
	
}

void AABPlayerController::StopRun()
{
	//APawn* const myPawn = GetPawn();
	//AABCharacter* myCharacter = Cast<AABCharacter>(myPawn);

	//myCharacter->GetCharacterMovement()->MaxWalkSpeed /= myCharacter->fSprintSpeedMultiPlayer;


	if (myCharacter->CurrentState == ECharacterState::READY && bCanRun == true)
	{
		if (myCharacter == nullptr) return;
		//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("PlayerStopRun!"));

		CtoS_StopRun(myCharacter);
		//MyStopRun.Broadcast();
	}
}

void AABPlayerController::CtoS_StopRun_Implementation(AABCharacter* ClientCharacter)
{
	// ���������� ��� PlayerController���� �̺�Ʈ�� ������.
	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(GetPawn()->GetWorld(), APlayerController::StaticClass(), OutActors);
	for (AActor* OutActor : OutActors)
	{
		AABPlayerController* PC = Cast<AABPlayerController>(OutActor);
		if (PC)
		{
			PC->StoC_StopRun(ClientCharacter);
		}
	}
}

void AABPlayerController::StoC_StopRun_Implementation(AABCharacter* ClientCharacter)
{
	// ������ Ŭ���̾�Ʈ�� �� �̺�Ʈ�� �޾Ƽ� �����Ѵ�.
	if (ClientCharacter == nullptr) return;

	if (bCanRun == false)
	{
		myCharacter->GetCharacterMovement()->MaxWalkSpeed = 200.0f;
	}

	ClientCharacter->GetCharacterMovement()->MaxWalkSpeed /= ClientCharacter->fSprintSpeedMultiPlayer;

}
// ä��

void AABPlayerController::SendMessage(const FText& Text)
{
	// GameInstance�� �����صξ��� �� �г���.
	UABGameInstance* MyGI = GetGameInstance<UABGameInstance>(); // GameInstance�� ���� ���� ���
	if (MyGI) 
   {
		FString UserName = MyGI->GetUserName("Player");
		FString Message = FString::Printf(TEXT("%s : %s"), *UserName, *Text.ToString());

		CtoS_SendMessage(Message); // �������� ����� �� �ֵ��� ������.
   }
	//FString Message = FString::Printf(TEXT("%s"), *Text.ToString());

	//CtoS_SendMessage(Message); // �������� ����� �� �ֵ��� ������.
}

void AABPlayerController::FocusChatInputText()
{
	AGameMain_HUD* HUD = GetHUD<AGameMain_HUD>();
	if (HUD == nullptr) return;

	FInputModeUIOnly InputMode;
	InputMode.SetWidgetToFocus(HUD->GetChatInputTextObject());

	SetInputMode(InputMode);
}

void AABPlayerController::FocusGame()
{
	SetInputMode(FInputModeGameOnly());
}

// �̼�

void AABPlayerController::SetPlayerMissionClear(int nMissionClear)
{
	AGameMain_HUD* HUD = GetHUD<AGameMain_HUD>();
	if (HUD == nullptr) return;
	HUD->SetPlayerMissionClear(nMissionClear);
}

void AABPlayerController::PlayerKillingMissionAppear()
{
	AGameMain_HUD* HUD = GetHUD<AGameMain_HUD>();
	if (HUD == nullptr) return;
	HUD->PlayerKillingMissionAppear();
}

void AABPlayerController::SetPlayerKillingPoint(int nKillingPoint)
{
	AGameMain_HUD* HUD = GetHUD<AGameMain_HUD>();
	if (HUD == nullptr) return;
	HUD->SetPlayerKillingPoint(nKillingPoint);
}

// ���� ��

void AABPlayerController::SetWinnerName(const FString& WinnerName)
{
	AGameMain_HUD* HUD = GetHUD<AGameMain_HUD>();
	if (HUD == nullptr) return;
	HUD->SetWinnerName(WinnerName);
}

void AABPlayerController::VisibleGameover()
{
	AGameMain_HUD* HUD = GetHUD<AGameMain_HUD>();
	if (HUD == nullptr) return;
	HUD->VisibleGameover();
}

void AABPlayerController::CtoS_GameEnd_Implementation(const FString& WinnerName)
{
	// ���������� ��� PlayerController���� �̺�Ʈ�� ������.
	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(GetPawn()->GetWorld(), APlayerController::StaticClass(), OutActors);
	for (AActor* OutActor : OutActors)
	{
		AABPlayerController* PC = Cast<AABPlayerController>(OutActor);
		if (PC)
		{
			PC->StoC_GameEnd(WinnerName);
		}
	}
}

void AABPlayerController::StoC_GameEnd_Implementation(const FString& WinnerName)
{
	AGameMain_HUD* HUD = GetHUD<AGameMain_HUD>();

	if (HUD == nullptr) return;

	HUD->SetWinnerName(WinnerName);
	HUD->VisibleGameover();
	SetShowMouseCursor(true);
}

void AABPlayerController::GameEnd(const FString& WinnerName)
{
	CtoS_GameEnd(WinnerName);
}

// ä��(����)

void AABPlayerController::CtoS_SendMessage_Implementation(const FString& Message)
{
	// ���������� ��� PlayerController���� �̺�Ʈ�� ������.
	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(GetPawn()->GetWorld(), APlayerController::StaticClass(), OutActors);
	for (AActor* OutActor : OutActors)
	{
		AABPlayerController* PC = Cast<AABPlayerController>(OutActor);
		if (PC)
		{
			PC->StoC_SendMessage(Message);
		}
	}
}

void AABPlayerController::StoC_SendMessage_Implementation(const FString& Message)
{
	// ������ Ŭ���̾�Ʈ�� �� �̺�Ʈ�� �޾Ƽ� �����Ѵ�.
	AGameMain_HUD* HUD = GetHUD<AGameMain_HUD>();
	if (HUD == nullptr) return;

	HUD->AddChatMessage(Message);
}

// ����(����)

void AABPlayerController::Attack()
{
	//APawn* const myPawn = GetPawn();
	//AABCharacter* myCharacter = Cast<AABCharacter>(myPawn);
	// ���� ���̸� �ٽ� ���ݸ��ϰ���

	if (bCanDeligate == false)
	{
		myCharacter->ABAnim->OnMontageEnded.AddDynamic(this, &AABPlayerController::OnAttackMontageEnded);
		bCanDeligate = true;
	}
	if (myCharacter->IsAttacking) return;

	if (myCharacter->CurrentState == ECharacterState::READY)
	{
		if (myCharacter == nullptr) return;
		UAnimMontage* playPunch;

		// �� �κп��� ���� ��Ÿ�ָ� �����Ѵ�.
		playPunch = myCharacter->ABAnim->GetAttackMontage();
		myCharacter->AttackPower = 100.0f;
		myCharacter->ABAnim->PlayAttackMontage(playPunch);
		myCharacter->IsAttacking = true;
		
		CtoS_Attack(myCharacter, playPunch);

		//FString testPlayerCount = FString::FromInt(myCharacter->nNowPlayer);
		//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, testPlayerCount);
		//MyStopRun.Broadcast();
	}
}

void AABPlayerController::CtoS_Attack_Implementation(AABCharacter* ClientCharacter, UAnimMontage* playPunch)
{
	// ���������� ��� PlayerController���� �̺�Ʈ�� ������.
	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(GetPawn()->GetWorld(), APlayerController::StaticClass(), OutActors);
	for (AActor* OutActor : OutActors)
	{
		AABPlayerController* PC = Cast<AABPlayerController>(OutActor);
		if (PC)
		{
			PC->StoC_Attack(ClientCharacter, playPunch);
		}
	}
}

void AABPlayerController::StoC_Attack_Implementation(AABCharacter* ClientCharacter, UAnimMontage* playPunch)
{
	// ������ Ŭ���̾�Ʈ�� �� �̺�Ʈ�� �޾Ƽ� �����Ѵ�.
	if (ClientCharacter->IsAttacking) return;

	ClientCharacter->AttackPower = 100.0f;
	ClientCharacter->ABAnim->PlayAttackMontage(playPunch);
	ClientCharacter->IsAttacking = true;

	/*
	if (ClientCharacter->DeathCharacter != NULL)
	{
		ClientCharacter->DeathCharacter->ABAnim->SetDeadAnim();
		//ClientCharacter->DeathCharacter->SetCharacterState(ECharacterState::DEAD);
		ClientCharacter->DeathCharacter->SetActorEnableCollision(false);
	    ClientCharacter->DeathCharacter = NULL;
	}
	*/
}

// ���ø�����Ʈ


void AABPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AABPlayerController, myCharacter);
	DOREPLIFETIME(AABPlayerController, myPawn);
	DOREPLIFETIME(AABPlayerController, bCanRun);
	DOREPLIFETIME(AABPlayerController, bMissionClear);
}
