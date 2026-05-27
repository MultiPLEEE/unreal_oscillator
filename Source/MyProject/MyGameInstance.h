// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MyGameInstance.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FArrayWrapper
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "Oscillator|Math")
    TArray<float> XValues;
    
    UPROPERTY(BlueprintReadWrite, Category = "Oscillator|Math")
    TArray<float> YValues;
    
    UPROPERTY(BlueprintReadWrite, Category = "Oscillator|Math")
    FText Title;
    
    UPROPERTY(BlueprintReadWrite, Category = "Oscillator|Math")
    FText XMeasure;
    
    UPROPERTY(BlueprintReadWrite, Category = "Oscillator|Math")
    FString YMeasure;
    
    UPROPERTY(BlueprintReadWrite, Category = "Oscillator|Math")
    bool IsPhasePortrait;
};

USTRUCT(BlueprintType)
struct FGraphWrapper
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "Oscillator|Math")
    TArray<FArrayWrapper> Graphs;
    
    UPROPERTY(BlueprintReadWrite, Category = "Oscillator|Math")
    float Frequency;
    
    UPROPERTY(BlueprintReadWrite, Category = "Oscillator|Math")
    float Period;
};

UCLASS()
class MYPROJECT_API UMyGameInstance : public UGameInstance
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Oscillator|Math")
    void PrepareArrays(int Size);
    
    UFUNCTION(BlueprintCallable, Category = "Oscillator|Math")
    int GetSize();
    
    UFUNCTION(BlueprintPure, Category = "Oscillator|Math")
    void GetAllValuesAtTarget(int Target, float& OutMass, float& OutStiffness, float& OutX0, float& OutRestLength, float& OutV0, float& OutFixedDeltaTime);
    
    
    
    UPROPERTY(BlueprintReadWrite, Category = "Oscillator|Math")
    int Page;
    
    UFUNCTION(BlueprintCallable, Category = "Oscillator|Math")
    void SetPage(int TargetPage);
    
    
    
    UPROPERTY(BlueprintReadWrite, Category = "Oscillator|Math")
    TArray<float> Mass;
    
    UPROPERTY(BlueprintReadWrite, Category = "Oscillator|Math")
    TArray<float> Stiffnes;
    
    UPROPERTY(BlueprintReadWrite, Category = "Oscillator|Math")
    TArray<float> X0;
    
    UPROPERTY(BlueprintReadWrite, Category = "Oscillator|Math")
    TArray<float> Restlength;
    
    UPROPERTY(BlueprintReadWrite, Category = "Oscillator|Math")
    TArray<float> V0;
    
    UPROPERTY(BlueprintReadWrite, Category = "Oscillator|Math")
    TArray<float> FixedDeltaTime;
    
    UPROPERTY(BlueprintReadWrite, Category = "Oscillator|Math")
    TArray<float> TestXValues;
    
    UPROPERTY(BlueprintReadWrite, Category = "Oscillator|Math")
    TArray<float> TestYValues;
    
    UPROPERTY(BlueprintReadWrite, Category = "Oscillator|Math")
    TArray<FGraphWrapper> AllGraphsArray;
    
    
    
    UFUNCTION(BlueprintCallable, Category = "Oscillator|Math")
    void AddMassValue(float NewValue, int Target);
    
    UFUNCTION(BlueprintCallable, Category = "Oscillator|Math")
    void AddStiffnesValue(float NewValue, int Target);
    
    UFUNCTION(BlueprintCallable, Category = "Oscillator|Math")
    void AddX0Value(float NewValue, int Target);
    
    UFUNCTION(BlueprintCallable, Category = "Oscillator|Math")
    void AddRestLengthValue(float NewValue, int Target);
    
    UFUNCTION(BlueprintCallable, Category = "Oscillator|Math")
    void AddV0Value(float NewValue, int Target);
    
    UFUNCTION(BlueprintCallable, Category = "Oscillator|Math")
    void AddFixedDeltaTime(float NewValue, int Target);
    
    UFUNCTION(BlueprintCallable, Category = "Oscillator|Math")
    void SetTime(float& Time, float NowTime, float LastTime, bool IsAllTime);
    
    UFUNCTION(BlueprintCallable, Category = "Oscillator|Math")
    void AddToAllGraphArray(int Index, float Time, float CurrentX_Ana, float Error, float CurrentX_Num, float CurrentV_Num, float Frequency, float Period);
    
    UFUNCTION(BlueprintCallable, Category = "Oscillator|Math")
    void SetRange(FArrayWrapper& SetAllArray, FArrayWrapper PreSetAllArray, float Range, bool IsAllTime, bool IsPhasePortrait);
};
