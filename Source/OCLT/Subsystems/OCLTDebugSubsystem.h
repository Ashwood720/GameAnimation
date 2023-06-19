// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "OCLTDebugSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class OCLT_API UOCLTDebugSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	bool IsCategoryEnabled(const FName& CategoryName) const;

private:
	UFUNCTION(Exec)
		void EnableDebugCategory(const FName& CategoryName, bool bIsEnabled);

	TMap<FName, bool> EnableDebugCategories;

};
