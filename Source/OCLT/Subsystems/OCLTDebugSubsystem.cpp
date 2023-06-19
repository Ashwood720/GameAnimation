// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/OCLTDebugSubsystem.h"

bool UOCLTDebugSubsystem::IsCategoryEnabled(const FName& CategoryName) const
{
	const bool* bIsEnabled = EnableDebugCategories.Find(CategoryName);
	return bIsEnabled != nullptr && *bIsEnabled;
}

void UOCLTDebugSubsystem::EnableDebugCategory(const FName& CategoryName, bool bIsEnabled)
{
	EnableDebugCategories.FindOrAdd(CategoryName);
	EnableDebugCategories[CategoryName] = bIsEnabled;
}