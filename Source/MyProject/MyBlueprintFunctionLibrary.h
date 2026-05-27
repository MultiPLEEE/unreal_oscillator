// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Blueprint/UserWidget.h" // Обязательно для FPaintContext
#include "MyBlueprintFunctionLibrary.generated.h"

UCLASS()
class MYPROJECT_API UMyBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Painting")
    static void DrawTextRotated(UPARAM(ref) FPaintContext& Context, FString Text, FVector2D Position, int FontSize, float AngleDeg, FLinearColor Color);
};
