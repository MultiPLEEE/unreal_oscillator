// Fill out your copyright notice in the Description page of Project Settings.


#include "MyStaticActor.h"

// Sets default values
AMyStaticActor::AMyStaticActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    
    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MyMesh"));
    RootComponent = MeshComponent;

    // Путь к стандартному кубу движка
    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
    if (MeshAsset.Succeeded())
    {
        MeshComponent->SetStaticMesh(MeshAsset.Object);
    }
}

// Called when the game starts or when spawned
void AMyStaticActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyStaticActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

