// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"

void UMyGameInstance::PrepareArrays(int Size)
{
    Mass.Init(10.0, Size);
    Stiffnes.Init(100.0, Size);
    X0.Init(50.0, Size);
    Restlength.Init(150.0, Size);
    V0.Init(0.0, Size);
    FixedDeltaTime.Init(0.01, Size);
    
    Page = 1;
}

void UMyGameInstance::SetPage(int TargetPage)
{
    Page = (TargetPage);
}

int UMyGameInstance::GetSize()
{
    return Mass.Num();
}

void UMyGameInstance::GetAllValuesAtTarget(int Target, float& OutMass, float& OutStiffness, float& OutX0, float& OutRestLength, float& OutV0, float& OutFixedDeltaTime)
{
    OutMass = Mass[Target-1];
    OutStiffness = Stiffnes[Target-1];
    OutX0 = X0[Target-1];
    OutRestLength = Restlength[Target-1];
    OutV0 = V0[Target-1];
    OutFixedDeltaTime = FixedDeltaTime[Target-1];
}

void UMyGameInstance::AddMassValue(float NewValue, int Target)
{
    Mass[Target-1] = (NewValue);
}

void UMyGameInstance::AddStiffnesValue(float NewValue, int Target)
{
    Stiffnes[Target-1] = (NewValue);
}

void UMyGameInstance::AddX0Value(float NewValue, int Target)
{
    X0[Target-1] = (NewValue);
}

void UMyGameInstance::AddRestLengthValue(float NewValue, int Target)
{
    Restlength[Target-1] = (NewValue);
}

void UMyGameInstance::AddV0Value(float NewValue, int Target)
{
    V0[Target-1] = (NewValue);
}

void UMyGameInstance::AddFixedDeltaTime(float NewValue, int Target)
{
    FixedDeltaTime[Target-1] = (NewValue);
}

void UMyGameInstance::SetTime(float& Time, float NowTime, float LastTime, bool IsAllTime)
{
    if (IsAllTime) { Time = 0; } else { Time = NowTime - LastTime; }
}

void UMyGameInstance::AddToAllGraphArray(int Index, float Time, float CurrentX_Ana, float Error, float CurrentX_Num, float CurrentV_Num, float Frequency, float Period)
{
    if (!AllGraphsArray.IsValidIndex(Index))
    {
        AllGraphsArray.SetNum(Index + 1);
    }

    FGraphWrapper& CurrentGraphContainer = AllGraphsArray[Index];

    if (CurrentGraphContainer.Graphs.Num() != 6)
    {
        CurrentGraphContainer.Graphs.SetNum(6);
        
        CurrentGraphContainer.Graphs[0].Title = INVTEXT("АНАЛ. ВЫСОТА ГРУЗИКА (ОТНОС. ПРУЖИНЫ)");
        CurrentGraphContainer.Graphs[0].XMeasure = INVTEXT("Время, с");
        CurrentGraphContainer.Graphs[0].YMeasure = TEXT("Положение, м");
        CurrentGraphContainer.Graphs[0].IsPhasePortrait = false;

        CurrentGraphContainer.Graphs[1].Title = INVTEXT("ПОГРЕШНОСТЬ");
        CurrentGraphContainer.Graphs[1].XMeasure = INVTEXT("Время, с");
        CurrentGraphContainer.Graphs[1].YMeasure = TEXT("Отклонение, м");
        CurrentGraphContainer.Graphs[1].IsPhasePortrait = false;

        CurrentGraphContainer.Graphs[2].Title = INVTEXT("ВЫСОТА ГРУЗИКА (ОТНОС. ПРУЖИНЫ)");
        CurrentGraphContainer.Graphs[2].XMeasure = INVTEXT("Время, с");
        CurrentGraphContainer.Graphs[2].YMeasure = TEXT("Положение, м");
        CurrentGraphContainer.Graphs[2].IsPhasePortrait = false;

        CurrentGraphContainer.Graphs[3].Title = INVTEXT("СКОРОСТЬ ГРУЗИКА");
        CurrentGraphContainer.Graphs[3].XMeasure = INVTEXT("Время, с");
        CurrentGraphContainer.Graphs[3].YMeasure = TEXT("Скорость, м/с");
        CurrentGraphContainer.Graphs[3].IsPhasePortrait = false;
        
        CurrentGraphContainer.Graphs[4].Title = INVTEXT("ФАЗОВЫЙ ПОРТРЕТ");
        CurrentGraphContainer.Graphs[4].XMeasure = INVTEXT("Положение, м");
        CurrentGraphContainer.Graphs[4].YMeasure = TEXT("Скорость, м/с");
        CurrentGraphContainer.Graphs[4].IsPhasePortrait = true;
        
        CurrentGraphContainer.Graphs[5].Title = INVTEXT("АМПЛИТУДА");
        CurrentGraphContainer.Graphs[5].XMeasure = INVTEXT("Время, с");
        CurrentGraphContainer.Graphs[5].YMeasure = TEXT("Отклонение, м");
        CurrentGraphContainer.Graphs[5].IsPhasePortrait = false;
        
        CurrentGraphContainer.Frequency = Frequency;
        CurrentGraphContainer.Period = Period;
    }

    CurrentGraphContainer.Graphs[0].XValues.Add(Time);
    CurrentGraphContainer.Graphs[0].YValues.Add(CurrentX_Ana);

    CurrentGraphContainer.Graphs[1].XValues.Add(Time);
    CurrentGraphContainer.Graphs[1].YValues.Add(Error);

    CurrentGraphContainer.Graphs[2].XValues.Add(Time);
    CurrentGraphContainer.Graphs[2].YValues.Add(CurrentX_Num);

    CurrentGraphContainer.Graphs[3].XValues.Add(Time);
    CurrentGraphContainer.Graphs[3].YValues.Add(CurrentV_Num);
    
    CurrentGraphContainer.Graphs[4].XValues.Add(CurrentX_Num);
    CurrentGraphContainer.Graphs[4].YValues.Add(CurrentV_Num);
    
    CurrentGraphContainer.Graphs[5].XValues.Add(Time);
    CurrentGraphContainer.Graphs[5].YValues.Add(FMath::Abs(CurrentX_Num));
}

void UMyGameInstance::SetRange(FArrayWrapper& SetAllArray, FArrayWrapper PreSetAllArray, float Range, bool IsAllTime, bool IsPhasePortrait)
{
    if (IsAllTime || IsPhasePortrait)
    {
        SetAllArray = PreSetAllArray;
        
        float LowXValue = SetAllArray.XValues[0];
        
        for (int i = 0; i < SetAllArray.XValues.Num(); i++) { SetAllArray.XValues[i] -= LowXValue; }
    }
    else
    {
        SetAllArray.XValues.Empty();
        SetAllArray.YValues.Empty();
        
        float LastX = PreSetAllArray.XValues.Last();
        float Threshold = LastX - Range;
        
        for (int i = 0; i < PreSetAllArray.XValues.Num(); ++i)
        {
            if (PreSetAllArray.XValues[i] >= Threshold)
            {
                SetAllArray.XValues.Add(PreSetAllArray.XValues[i] - Threshold);
                SetAllArray.YValues.Add(PreSetAllArray.YValues[i]);
            }
        }
        
        float LowXValue;
        if (SetAllArray.XValues[0] - SetAllArray.XValues[0] - Threshold < 0) { LowXValue = SetAllArray.XValues[0]; } else { LowXValue = SetAllArray.XValues[0] + Threshold; }
        
        for (int i = 0; i < SetAllArray.XValues.Num(); i++) { SetAllArray.XValues[i] -= LowXValue; }
    
        SetAllArray.Title = PreSetAllArray.Title;
        SetAllArray.XMeasure = PreSetAllArray.XMeasure;
        SetAllArray.YMeasure = PreSetAllArray.YMeasure;
        SetAllArray.IsPhasePortrait = PreSetAllArray.IsPhasePortrait;
    }
}
