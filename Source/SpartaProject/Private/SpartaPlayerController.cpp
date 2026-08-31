
#include "SpartaPlayerController.h"
#include "SpartaGameState.h"
#include "SpartaGameInstance.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"

ASpartaPlayerController::ASpartaPlayerController()
	:inputMappingContext(nullptr),
	MoveAction(nullptr),
	JumpAction(nullptr),
	LookAction(nullptr),
	SprintAction(nullptr),
	HUDWidgetClass(nullptr),
	HUDWidgetInstance(nullptr),
	MainMenuWidgetClass(nullptr),
	MainMenuWidgetInstance(nullptr),
	GameOverWidgetClass(nullptr),
	GameOverWidgetInstance(nullptr)
{
}

void ASpartaPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* SubSystem =
			LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (inputMappingContext)
			{
				SubSystem->AddMappingContext(inputMappingContext, 0);
			}
		}
	}

	FString CurrentMapName = GetWorld()->GetMapName();
	if (CurrentMapName.Contains("MenuLevel"))
	{
		ShowMainMenu(false);
	}
}

UUserWidget* ASpartaPlayerController::GetHUDWidget()const
{
	return HUDWidgetInstance;
}

void ASpartaPlayerController::ShowMainMenu(bool bIsRestart)
{
	if (HUDWidgetInstance)
	{
		HUDWidgetInstance->RemoveFromParent();
		HUDWidgetInstance = nullptr;
	}

	if (MainMenuWidgetInstance)
	{
		MainMenuWidgetInstance->RemoveFromParent();
		MainMenuWidgetInstance = nullptr;
	}

	if (GameOverWidgetInstance)
	{
		GameOverWidgetInstance->RemoveFromParent();
		GameOverWidgetInstance = nullptr;
	}

	TSubclassOf<UUserWidget> WidgetClassToUse = bIsRestart ? GameOverWidgetClass : MainMenuWidgetClass;

	if (WidgetClassToUse)
	{
		if (bIsRestart)
		{
			GameOverWidgetInstance = CreateWidget<UUserWidget>(this, GameOverWidgetClass);
		}
		else
		{
			MainMenuWidgetInstance = CreateWidget<UUserWidget>(this, MainMenuWidgetClass);
		}

		UUserWidget* WidgetInstanceToUse = bIsRestart ? GameOverWidgetInstance : MainMenuWidgetInstance;

		if (WidgetInstanceToUse)
		{
			WidgetInstanceToUse->AddToViewport();

			bShowMouseCursor = true;
			SetInputMode(FInputModeUIOnly());


			if (UTextBlock* ButtonText = Cast<UTextBlock>(WidgetInstanceToUse->GetWidgetFromName(TEXT("StartButtonText"))))
			{
				if (bIsRestart)
				{
					ButtonText->SetText(FText::FromString(TEXT("Restart")));
				}
				else
				{
					ButtonText->SetText(FText::FromString(TEXT("Start")));
				}
			}
			if (bIsRestart)
			{
				UFunction* PlayAnimFuc = WidgetInstanceToUse->FindFunction(FName("PlayGameOverAnim"));
				if (PlayAnimFuc)
				{
					WidgetInstanceToUse->ProcessEvent(PlayAnimFuc, nullptr);
				}

				if (UTextBlock* TotalScoreText = Cast<UTextBlock>(WidgetInstanceToUse->GetWidgetFromName("TotalScoreText")))
				{
					if (USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(UGameplayStatics::GetGameInstance(this)))
					{
						TotalScoreText->SetText(FText::FromString(
							FString::Printf(TEXT("Total Score: %d"), SpartaGameInstance->TotalScore)
						));
					}
				}
			}
		}
	}
}
void ASpartaPlayerController::ShowGameHUD()
{
	if (HUDWidgetInstance)
	{
		HUDWidgetInstance->RemoveFromParent();
		HUDWidgetInstance = nullptr;
	}

	if (MainMenuWidgetInstance)
	{
		MainMenuWidgetInstance->RemoveFromParent();
		MainMenuWidgetInstance = nullptr;
	}

	if (HUDWidgetClass)
	{
		HUDWidgetInstance = CreateWidget<UUserWidget>(this, HUDWidgetClass);
		if (HUDWidgetInstance)
		{
			HUDWidgetInstance->AddToViewport();

			bShowMouseCursor = false;
			SetInputMode(FInputModeGameOnly());
		}
		
		ASpartaGameState* SpartaGameState = GetWorld() ? GetWorld()->GetGameState<ASpartaGameState>() : nullptr;
			if (SpartaGameState)
			{
				SpartaGameState->UpdateHUD();
			}
	}
}

void ASpartaPlayerController::StartGame()
{
	if (USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(UGameplayStatics::GetGameInstance(this)))
	{
		SpartaGameInstance->CurrentLevelIndex = 0;
		SpartaGameInstance->TotalScore = 0;
	}

	UGameplayStatics::OpenLevel(GetWorld(), FName("BasicLevel"));
	SetPause(false);

}

void ASpartaPlayerController::ReturnToMainMenu()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName("ManuLevel"));
}