// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterPlayerController.h"
#include "TimerManager.h"
#include "Blueprint/UserWidget.h"

void AShooterPlayerController::GameHasEnded(class AActor* EndGameFocus, bool bIsWinner)
{
    Super::GameHasEnded(EndGameFocus, bIsWinner);

    // UE_LOG(LogTemp, Warning, TEXT("We have finished!"));

    // Create variable of UserWidget
    UUserWidget* LoseScreen = CreateWidget(this, LoseScreenClass);
    if (LoseScreen != nullptr)
    {
        // Adding Screen
        LoseScreen->AddToViewport();
    }
    

    // Function to set delay for restart the game when the player died
    GetWorldTimerManager().SetTimer(RestartTimer, this, &APlayerController::RestartLevel, RestartDelay);
}
