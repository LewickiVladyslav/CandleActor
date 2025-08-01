#include "Candle.h"

ACandle::ACandle()
{
	PrimaryActorTick.bCanEverTick = true;
	
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
	
	Wax = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wax"));
	Wax->SetupAttachment(RootComponent);
	
	Wick = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wick"));
	Wick->SetupAttachment(Wax); 
	
	CandleLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("CandleLight"));
	CandleLight->SetupAttachment(Wick);
	CandleLight->bUseInverseSquaredFalloff = true;

	Flame = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Flame"));
	Flame->SetupAttachment(Wick);
	Flame->SetRelativeLocation(FVector(0.0f, 0.0f, 5.0f)); 

	Smoke = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Smoke"));
	Smoke->SetupAttachment(Wick);
	Smoke->SetRelativeLocation(FVector(0.0f, 0.0f, 8.0f));

	DistortionStr = 0.5f;
	bWasFlameActive = true;
	PreviousDistortionStr = -1.0f; 
}

void ACandle::BeginPlay()
{
	Super::BeginPlay();
	UpdateMaterialParameter();

	if (DistortionStr > 8.1f)
	{
		if (Flame) Flame->SetVisibility(false);
		if (Smoke) Smoke->SetVisibility(true);
		if (CandleLight) CandleLight->SetVisibility(false);
		bWasFlameActive = false;
	}
	else
	{
		if (Flame) Flame->SetVisibility(true);
		if (Smoke) Smoke->SetVisibility(false);
		if (CandleLight) CandleLight->SetVisibility(true);
		bWasFlameActive = true;
	}
}

void ACandle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (FMath::Abs(DistortionStr - PreviousDistortionStr) > KINDA_SMALL_NUMBER)
	{
		UpdateMaterialParameter();
		PreviousDistortionStr = DistortionStr;
	}

	bool bShouldFlameBeActive = DistortionStr <= 8.1f;
	
	if (bShouldFlameBeActive != bWasFlameActive)
	{
		if (bShouldFlameBeActive)
		{
			if (Flame) 
			{
				Flame->SetVisibility(true);
				Flame->Activate();
			}
			if (Smoke) 
			{
				Smoke->SetVisibility(false);
				Smoke->Deactivate();
			}
			if (CandleLight) CandleLight->SetVisibility(true);
		}
		else
		{
			if (Flame) 
			{
				Flame->SetVisibility(false);
				Flame->Deactivate();
			}
			if (Smoke) 
			{
				Smoke->SetVisibility(true);
				Smoke->Activate();
			}
			if (CandleLight) CandleLight->SetVisibility(false);
		}
		
		bWasFlameActive = bShouldFlameBeActive;
	}
}

void ACandle::UpdateMaterialParameter()
{
	if (MaterialParameterCollection)
	{
		UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), MaterialParameterCollection, TEXT("DistortionStr"), DistortionStr);
	}
}
