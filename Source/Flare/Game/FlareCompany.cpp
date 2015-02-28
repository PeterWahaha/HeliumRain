
#include "Flare.h"
#include "FlareGame.h"
#include "FlareCompany.h"
#include "../Ships/FlareShip.h"
#include "../Stations/FlareStation.h"


/*----------------------------------------------------
	Constructor
----------------------------------------------------*/

UFlareCompany::UFlareCompany(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}


/*----------------------------------------------------
	Save
----------------------------------------------------*/

void UFlareCompany::Load(const FFlareCompanySave& Data)
{
	CompanyData = Data;
	CompanyData.Identifier = FName(*GetName());
}

FFlareCompanySave* UFlareCompany::Save()
{
	return &CompanyData;
}

void UFlareCompany::Register(IFlareShipInterface* Ship)
{
	CompanyShips.AddUnique(Ship);
}

void UFlareCompany::Register(IFlareStationInterface* Station)
{
	CompanyStations.AddUnique(Station);
}

void UFlareCompany::Unregister(IFlareShipInterface* Ship)
{
	CompanyShips.Remove(Ship);
}

void UFlareCompany::Unregister(IFlareStationInterface* Station)
{
	CompanyStations.Remove(Station);
}


/*----------------------------------------------------
	Customization
----------------------------------------------------*/

inline void UFlareCompany::SetLightColorIndex(int32 Index)
{
	CompanyData.CustomizationLightColorIndex = Index;
	UpdateCompanyCustomization();
}

inline void UFlareCompany::SetPaintColorIndex(int32 Index)
{
	CompanyData.CustomizationPaintColorIndex = Index;
	UpdateCompanyCustomization();
}

inline void UFlareCompany::SetEngineColorIndex(int32 Index)
{
	CompanyData.CustomizationEngineColorIndex = Index;
	UpdateCompanyCustomization();
}

inline void UFlareCompany::SetPatternIndex(int32 Index)
{
	CompanyData.CustomizationPatternIndex = Index;
	UpdateCompanyCustomization();
}

void UFlareCompany::UpdateCompanyCustomization()
{
	// Update ships
	for (int32 i = 0; i < CompanyShips.Num(); i++)
	{
		AFlareShip* Ship = Cast<AFlareShip>(CompanyShips[i]);
		if (Ship)
		{
			Ship->UpdateCustomization();
		}
	}

	// Update stations
	for (int32 i = 0; i < CompanyStations.Num(); i++)
	{
		AFlareStation* Station = Cast<AFlareStation>(CompanyStations[i]);
		if (Station)
		{
			Station->UpdateCustomization();
		}
	}
}

void UFlareCompany::CustomizeModuleMaterial(UMaterialInstanceDynamic* Mat)
{
	// Get data from storage
	FLinearColor PaintColor = GetGame()->GetCustomizationCatalog()->GetColor(GetPaintColorIndex());
	FLinearColor LightColor = GetGame()->GetCustomizationCatalog()->GetColor(GetLightColorIndex());
	UTexture2D* Pattern = GetGame()->GetCustomizationCatalog()->GetPattern(GetPatternIndex());

	// Apply settings to the material instance
	Mat->SetVectorParameterValue("PaintColor", PaintColor);
	Mat->SetVectorParameterValue("GlowColor", NormalizeColor(LightColor));
	Mat->SetTextureParameterValue("PaintPattern", Pattern);
}

void UFlareCompany::CustomizeEffectMaterial(UMaterialInstanceDynamic* Mat)
{
	FLinearColor EngineColor = GetGame()->GetCustomizationCatalog()->GetColor(GetEngineColorIndex());
	Mat->SetVectorParameterValue("GlowColor", NormalizeColor(EngineColor));
}

FLinearColor UFlareCompany::NormalizeColor(FLinearColor Col) const
{
	return FLinearColor(FVector(Col.R, Col.G, Col.B) / Col.GetLuminance());
}
