// Fill out your copyright notice in the Description page of Project Settings.


#include "GameOscillator.h"
#include "MyGameInstance.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/TextRenderComponent.h"

#include <cmath>

// Sets default values
AGameOscillator::AGameOscillator()
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
    
    // 4. Делаю триггер для активации брызгов воды
    WaterTrigger = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WaterTrigger"));
    
    WaterTrigger->SetupAttachment(PayloadMesh);
    WaterTrigger->SetMobility(EComponentMobility::Movable);
}

// Called when the game starts or when spawned
void AGameOscillator::BeginPlay()
{
    Super::BeginPlay();

    // 1. Установка начальных условий
    CurrentX_Num = InX0;
    ElapsedTime = 0.0;
    
    // 2. Получение высоты пружинки
    FVector Min, Max;
    SpringVisual->GetLocalBounds(Min, Max);
    SpringFullHeight = Max.Z - Min.Z;
    
    // 3. Получение половины высоты грузика
    PayloadMesh->GetLocalBounds(Min, Max);
    PayloadHalfHeight = ((Max.Z - Min.Z) / 2.0 / 10.0);
}

// Called every frame
void AGameOscillator::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (!bRunSimulation) return;

    double dt = DeltaTime * InTimeScale;
    TimeAccumulator += dt;

    UMyGameInstance* MyGI = Cast<UMyGameInstance>(GetGameInstance());
    
    // TimeAccumulator накапливает время, которое после "разряжается" с шагом FixedDeltaTime, что позволяет сохранить точность графиков при скачках кадров
    while (TimeAccumulator >= InFixedDeltaTime)
    {
        Omega = std::sqrt(InStiffness / InMass);
        Period = 2.0 * PI * (1.0 / Omega);
        Frequency = 1.0 / Period;
        
        double Acceleration = -(InStiffness / InMass) * CurrentX_Num;
        
        // --- ЧИСЛЕННОЕ РЕШЕНИЕ (Метод Верле) ---
        double NewX = CurrentX_Num + InCurrentV_Num * InFixedDeltaTime + 0.5 * Acceleration * InFixedDeltaTime * InFixedDeltaTime;
        double NewAcceleration = -(InStiffness / InMass) * NewX;
        InCurrentV_Num += 0.5 * (Acceleration + NewAcceleration) * InFixedDeltaTime;
        CurrentX_Num = NewX;
        
        // --- "ИНКРЕМЕНТ" ВРЕМЕНИ ---
        ElapsedTime += InFixedDeltaTime;
        
        // --- АНАЛИТИЧЕСКОЕ РЕШЕНИЕ ---
        CurrentX_Ana = InX0 * std::cos(Omega * ElapsedTime);
        
        // --- ПОГРЕШНОСТЬ ---
        double Diff = CurrentX_Num - CurrentX_Ana;
        TotalSqError += Diff * Diff;
        
        // --- "РАЗРЯДКА" АККУМУЛЯТРА ---
        TimeAccumulator -= InFixedDeltaTime;
    }

    // --- АНИМАЦИЯ ---
    // Сдвигаем меш груза. -RestLength — это точка равновесия
    float VisualZ = -InRestLength + CurrentX_Num;
    PayloadMesh->SetRelativeLocation(FVector(0.0f, 0.0f, VisualZ));

    // Растягиваем пружину (Scale Z)
    float SpringScale = (VisualZ + PayloadHalfHeight) / SpringFullHeight;
    SpringVisual->SetRelativeScale3D(FVector(0.13f, 0.13f, SpringScale));

    // Выравниваем пружину, так как она растягивается в обе стороны (Scale Z)
    float SpringLocation = (VisualZ + PayloadHalfHeight) / 2.0f;
    SpringVisual->SetRelativeLocation(FVector(0.0f, 0.0f, SpringLocation));
}
