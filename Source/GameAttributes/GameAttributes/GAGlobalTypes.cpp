#pragma once
#include "GameAttributes.h"
#include "GameplayTagContainer.h"
#include "GAAttributeComponent.h"
#include "GAAttributesBase.h"
#include "IGAAttributes.h"
#include "Effects/GAEffect.h"
#include "Effects/GAEffectSpecification.h"
#include "GAAttributeComponent.h"
#include "GAAttributesBase.h"
#include "GAEffects.h"
#include "GAGlobalTypes.h"
FGAHashedGameplayTagContainer::FGAHashedGameplayTagContainer(const FGameplayTagContainer& TagsIn)
	: Tags(TagsIn)
{
	GenerateFNameKey();
}
void FGAHashedGameplayTagContainer::GenerateFNameKey()
{
	FString RetString;
	for (const FGameplayTag& tag : Tags)
	{
		RetString += TEXT(".");
		RetString += tag.ToString();
		RetString += TEXT(".");
	}
	Key = *RetString;
}
FGAEffectHandle FGAEffectHandle::GenerateHandle()
{
	static int32 GlobalHandleID = 0;
	FGAEffectHandle NewHandle(GlobalHandleID++);

	return NewHandle;
}
FGAModifier::FGAModifier(EGAAttributeMod ModIn, float ValueIn, TSharedPtr<struct FGAActiveDuration> ActiveEffect)
	: AttributeMod(ModIn),
	Value(ValueIn)
{
	Effect = TWeakPtr<FGAActiveDuration>(ActiveEffect);
}
void FGAEffectContext::Reset()
{
	Target.Reset();
	Causer.Reset();
	Instigator.Reset();
	TargetComp.Reset();
	InstigatorComp.Reset();
}
class UGAAttributesBase* FGAEffectContext::GetTargetAttributes()
{ 
	return TargetComp->DefaultAttributes; 
}
class UGAAttributesBase* FGAEffectContext::GetInstigatorAttributes()
{ 
	return InstigatorComp->DefaultAttributes; 
}
FGAEffectContext::~FGAEffectContext()
{
	Target.Reset();
	Causer.Reset();
	Instigator.Reset();
	TargetComp.Reset();
	InstigatorComp.Reset();
}

void FGACountedTagContainer::AddTag(const FGameplayTag& TagIn)
{
	int32* count = CountedTags.Find(TagIn);
	if (count)
	{
		*count += 1;
		return;
	}
	CountedTags.Add(TagIn, 1);
	AllTags.AddTag(TagIn);
}
void FGACountedTagContainer::AddTagContainer(const FGameplayTagContainer& TagsIn)
{
	for (auto TagIt = TagsIn.CreateConstIterator(); TagIt; ++TagIt)
	{
		int32* count = CountedTags.Find(*TagIt);
		if (count)
		{
			*count += 1;
		}
		else
		{
			CountedTags.Add(*TagIt, 1);
			AllTags.AddTag(*TagIt);
		}
	}
}
void FGACountedTagContainer::RemoveTag(const FGameplayTag& TagIn)
{
	int32* count = CountedTags.Find(TagIn);
	if (count)
	{
		*count -= 1;
		if (*count <= 0)
		{
			CountedTags.Remove(TagIn);
			AllTags.RemoveTag(TagIn);
		}
	}
}
bool FGACountedTagContainer::HasTag(const FGameplayTag& TagIn)
{
	return AllTags.HasTag(TagIn, EGameplayTagMatchType::IncludeParentTags, EGameplayTagMatchType::Explicit);
}
bool FGACountedTagContainer::HasAllTags(const FGameplayTagContainer& TagsIn, bool bCountEmptyAsMatch)
{
	return AllTags.MatchesAll(TagsIn, bCountEmptyAsMatch);
}
bool FGACountedTagContainer::HasAnyTags(const FGameplayTagContainer& TagsIn, bool bCountEmptyAsMatch)
{
	return AllTags.MatchesAny(TagsIn, bCountEmptyAsMatch);
}