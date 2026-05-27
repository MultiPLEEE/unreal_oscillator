// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBlueprintFunctionLibrary.h"
#include "Rendering/DrawElements.h"
#include "Fonts/SlateFontInfo.h"
#include "Framework/Application/SlateApplication.h"
#include "Fonts/FontMeasure.h"
#include "Styling/CoreStyle.h"  // Добавлено для FCoreStyle

void UMyBlueprintFunctionLibrary::DrawTextRotated(UPARAM(ref) FPaintContext& Context, FString Text, FVector2D Position, int FontSize, float AngleDeg, FLinearColor Color)
{
    FSlateFontInfo Font = FCoreStyle::Get().GetFontStyle(TEXT("NormalFont"));
    Font.Size = FontSize;
    
    TSharedRef<FSlateFontMeasure> FontMeasure = FSlateApplication::Get().GetRenderer()->GetFontMeasureService();
    FVector2f TextSize = FontMeasure->Measure(Text, Font);
    
    // Корректируем позицию, чтобы центр текста был в CenterPosition
    FVector2f TopLeftPosition = FVector2f(Position) - (TextSize * 0.5f);
    
    FPaintGeometry TextGeometry = Context.AllottedGeometry.ToPaintGeometry(
        TextSize,
        FSlateLayoutTransform(TopLeftPosition),
        FSlateRenderTransform(FQuat2D(FMath::DegreesToRadians(AngleDeg))),
        FVector2f(0.5f, 0.5f)  // Вращение вокруг центра
    );
    
    FSlateDrawElement::MakeText(
        Context.OutDrawElements,
        Context.MaxLayer,
        TextGeometry,
        Text,
        Font,
        ESlateDrawEffect::None,
        Color
    );
}
