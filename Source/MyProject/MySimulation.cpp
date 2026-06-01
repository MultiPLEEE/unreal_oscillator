// Fill out your copyright notice in the Description page of Project Settings.


#include "MySimulation.h"
#include "MyGameInstance.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/TextRenderComponent.h"

#include <cmath>

// Sets default values
AMySimulation::AMySimulation()
{
     // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    
    // 0. Делаю Анчор-рут для симуляции
    Anchor = CreateDefaultSubobject<USceneComponent>(TEXT("Anchor"));
    SetRootComponent(Anchor);
    
    // 1. Делаю Анчор-меш для симуляции
    AnchorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AnchorMesh"));
    
    AnchorMesh->SetupAttachment(Anchor);
    
    // 2. Делаю грузик для симуляции
    PayloadMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Payload"));
    
    PayloadMesh->SetupAttachment(Anchor);
    PayloadMesh->SetMobility(EComponentMobility::Movable);
    
    // 3. Делаю "Пружину" (пока просто тонкая капсула между ними)
    SpringVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SpringVisual"));
    
    SpringVisual->SetupAttachment(Anchor);
    SpringVisual->SetMobility(EComponentMobility::Movable);
    
    // 4. Делаю номер маятника для наглядности
    TextRender = CreateDefaultSubobject<UTextRenderComponent>(TEXT("MyTextRender"));
    
    TextRender->SetupAttachment(Anchor);
}

// Called when the game starts or when spawned
void AMySimulation::BeginPlay()
{
    Super::BeginPlay();
    
    TextRender->SetText(FText::FromString(FString::FromInt(Index)));
    
    // 1. Замена типов
    Mass = (double)InMass;
    Stiffness = (double)InStiffness;
    X0 = (double)InX0;
    RestLength = (double)InRestLength;
    CurrentV_Num = (double)InCurrentV_Num;
    FixedDeltaTime = (double)InFixedDeltaTime;
    TimeScale = (double)InTimeScale;
    
    // 2. Расчет характеристик
    Omega = std::sqrt(Stiffness / Mass);
    Period = 2.0 * PI * (1.0 / Omega);
    Frequency = 1.0 / Period;
    NIntegrationStepRecords = FMath::Max(FMath::RoundToInt((1.0 / APS) / FixedDeltaTime), 1);

    // 3. Установка начальных условий
    CurrentX_Num = X0;
    ElapsedTime = 0.0;
    
    // 4. Получение высоты пружинки
    FVector Min, Max;
    SpringVisual->GetLocalBounds(Min, Max);
    SpringFullHeight = Max.Z - Min.Z;
    
    // 5. Получение половины высоты грузика
    PayloadMesh->GetLocalBounds(Min, Max);
    PayloadHalfHeight = ((Max.Z - Min.Z) / 2.0 / 10.0);
}

// Called every frame
void AMySimulation::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (!bRunSimulation) return;

    double dt = DeltaTime * TimeScale;
    TimeAccumulator += dt;

    UMyGameInstance* MyGI = Cast<UMyGameInstance>(GetGameInstance());
    
    // TimeAccumulator накапливает время, которое после "разряжается" с шагом FixedDeltaTime, что позволяет сохранить точность графиков при скачках кадров
    while (TimeAccumulator >= FixedDeltaTime)
    {
        NIntegrationSteps++;
        
        double Acceleration = -(Stiffness / Mass) * CurrentX_Num;
        
        // --- ЧИСЛЕННОЕ РЕШЕНИЕ (Метод Эйлера-Кромера) ---
        // a = F/m = -k*x / m
//        CurrentV_Num += Acceleration * FixedDeltaTime;
//        CurrentX_Num += CurrentV_Num * FixedDeltaTime;
        
        // --- ЧИСЛЕННОЕ РЕШЕНИЕ (Метод Верле) ---
        double NewX = CurrentX_Num + CurrentV_Num * FixedDeltaTime + 0.5 * Acceleration * FixedDeltaTime * FixedDeltaTime;
        double NewAcceleration = -(Stiffness / Mass) * NewX;
        CurrentV_Num += 0.5 * (Acceleration + NewAcceleration) * FixedDeltaTime;
        CurrentX_Num = NewX;
        
        // --- "ИНКРЕМЕНТ" ВРЕМЕНИ ---
        ElapsedTime += FixedDeltaTime;
        
        // --- АНАЛИТИЧЕСКОЕ РЕШЕНИЕ ---
        CurrentX_Ana = X0 * std::cos(Omega * ElapsedTime);
        
        // --- ПОГРЕШНОСТЬ ---
        double Diff = CurrentX_Num - CurrentX_Ana;
        TotalSqError += Diff * Diff;
        NErrorSteps++;
        
        if (NIntegrationSteps % NIntegrationStepRecords == 0)
        {
            // --- ОКОНЧАТЕЛЬНАЯ ПОГРЕШНОСТЬ ---
            RMSError = std::sqrt((1.0 / NErrorSteps) * TotalSqError);
            
            // --- ОТПРАВКА ДАННЫХ В СПИСОК ДЛЯ ВЫВОДА ---
            MyGI->AddToAllGraphArray(Index - 1, (float)ElapsedTime, (float)CurrentX_Ana, (float)RMSError, (float)CurrentX_Num, (float)CurrentV_Num, (float)Frequency, (float)Period);
        }

        // --- "РАЗРЯДКА" АККУМУЛЯТРА ---
        TimeAccumulator -= FixedDeltaTime;
    }

    // --- АНИМАЦИЯ ---
    // Сдвигаем меш груза. -RestLength — это точка равновесия
    float VisualZ = -RestLength + CurrentX_Num;
    PayloadMesh->SetRelativeLocation(FVector(0.0f, 0.0f, VisualZ));

    // Растягиваем пружину (Scale Z)
    float SpringScale = (VisualZ + PayloadHalfHeight) / SpringFullHeight;
    SpringVisual->SetRelativeScale3D(FVector(0.13f, 0.13f, SpringScale));

    // Выравниваем пружину, так как она растягивается в обе стороны (Scale Z)
    float SpringLocation = (VisualZ + PayloadHalfHeight) / 2.0f;
    SpringVisual->SetRelativeLocation(FVector(0.0f, 0.0f, SpringLocation));
}
