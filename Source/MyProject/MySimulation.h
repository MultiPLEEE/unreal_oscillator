// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MySimulation.generated.h"

UCLASS()
class MYPROJECT_API AMySimulation : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMySimulation();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
    // Анчор-рут
    UPROPERTY(VisibleAnywhere, Category = "Oscillator|Setup")
    USceneComponent* Anchor;
    
    // Анчор-меш
    UPROPERTY(VisibleAnywhere, Category = "Oscillator|Setup")
    UStaticMeshComponent* AnchorMesh;

    // Груз (подвижная часть)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Oscillator|Setup")
    UStaticMeshComponent* PayloadMesh;

    // Слот для пружины (визуальный эффект)
    UPROPERTY(VisibleAnywhere, Category = "Oscillator|Setup")
    UStaticMeshComponent* SpringVisual;

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
    class UTextRenderComponent* TextRender;

public:    
    // --- ВХОДНЫЕ ДАННЫЕ (9 параметров) ---
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = "true"), Category = "Oscillator|Input")
    float InMass = 10.0;                // 1. m (кг)
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = "true"), Category = "Oscillator|Input")
    float InStiffness = 100.0;          // 2. k (Н/м)
     
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = "true"), Category = "Oscillator|Input")
    float InX0 = 50.0;                  // 3. Начальное отклонение (см)
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = "true"), Category = "Oscillator|Input")
    float InRestLength = 150.0;         // 4. Длина пружины в покое
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = "true"), Category = "Oscillator|Input")
    float InCurrentV_Num = 0.0;         // 5. Численная скорость
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = "true"), Category = "Oscillator|Input")
    float InFixedDeltaTime = 0.01;      // 6. Шаг интегрирования
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = "true"), Category = "Oscillator|Input")
    int Index = 0;                    // 7. Номер маятника
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = "true"), Category = "Oscillator|Input")
    float InTimeScale = 1.0;            // 8. Множитель времени (для замедления)
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = "true"), Category = "Oscillator|Input")
    bool bRunSimulation = true;       // 9. Флаг старта
    
    // --- ПОДГОТОВКА К ЗАМЕНЕ ТИПОВ ВХОДНЫХ ДАННЫХ ---
    double Mass;

    double Stiffness;

    double X0;

    double RestLength;

    double CurrentV_Num;

    double FixedDeltaTime;
    
    double TimeScale;

    // --- ВЫХОДНЫЕ ДАННЫЕ (Характеристики) ---
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Oscillator|Output")
    double Period;                    // T = 2*pi*sqrt(m/k)
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Oscillator|Output")
    double Frequency;                 // f = 1/T
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Oscillator|Output")
    double RMSError = 0;              // Среднеквадратичная погрешность (Num - Ana)

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Oscillator|Output")
    double CurrentX_Num;              // Численное положение
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Oscillator|Output")
    double CurrentX_Ana;              // Аналитическое положение
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Oscillator|Output")
    double ElapsedTime = 0.0;
    
    // --- ОСТАЛЬНЫЕ ПЕРЕМЕННЫЕ ДЛЯ ВЫЧИСЛЕНИЙ ---
    double SpringFullHeight;
    
    double PayloadHalfHeight;
    
    double Omega;
    
    double TimeAccumulator = 0.0;
    
    double TotalSqError = 0;
    
    int NErrorSteps = 0;
    
    int NIntegrationSteps = 0;
    
    int NIntegrationStepRecords;
    
    int APS = 15;
    
    
    
    UPROPERTY()
    class UMyGameInstance* CachedGI;
};
