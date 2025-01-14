// Author: Lucas Vilas-Boas
// Year: 2023
// Repo: https://github.com/lucoiso/UEHttpGPT

#include "HttpGPTSettings.h"
#include "LogHttpGPT.h"
#include <Runtime/Launch/Resources/Version.h>

#ifdef UE_INLINE_GENERATED_CPP_BY_NAME
#include UE_INLINE_GENERATED_CPP_BY_NAME(HttpGPTSettings)
#endif

UHttpGPTSettings::UHttpGPTSettings(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer), bEnableInternalLogs(false)
{
	CategoryName = TEXT("Plugins");

	SetToDefaults();
}

const UHttpGPTSettings* UHttpGPTSettings::Get()
{
	const UHttpGPTSettings* const Instance = GetDefault<UHttpGPTSettings>();
	return Instance;
}

FHttpGPTOptions UHttpGPTSettings::GetDefaultOptions()
{
	return GetDefault<UHttpGPTSettings>()->DefaultOptions;
}

void UHttpGPTSettings::SetDefaultOptions(const FHttpGPTOptions& Value)
{
	UHttpGPTSettings* const Settings = GetMutableDefault<UHttpGPTSettings>();
	Settings->DefaultOptions = Value;

	Settings->SaveAndReload(GET_MEMBER_NAME_CHECKED(UHttpGPTSettings, DefaultOptions));
}

#if WITH_EDITOR
void UHttpGPTSettings::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.Property->GetFName() == GET_MEMBER_NAME_CHECKED(UHttpGPTSettings, bEnableInternalLogs))
	{
		ToggleInternalLogs();
	}
}
#endif

void UHttpGPTSettings::PostInitProperties()
{
	Super::PostInitProperties();
	ToggleInternalLogs();
}

void UHttpGPTSettings::SetToDefaults()
{
	DefaultOptions.APIKey = NAME_None;
	DefaultOptions.Model = EHttpGPTModel::gpt35turbo;
	DefaultOptions.MaxTokens = 2048;
	DefaultOptions.Temperature = 1.f;
	DefaultOptions.TopP = 1.f;
	DefaultOptions.Choices = 1;
	DefaultOptions.bStream = true;
	DefaultOptions.Stop = TArray<FName>();
	DefaultOptions.PresencePenalty = 0.f;
	DefaultOptions.FrequencyPenalty = 0.f;
	DefaultOptions.LogitBias = TMap<int32, float>();
	DefaultOptions.User = NAME_None;
}

void UHttpGPTSettings::SaveAndReload(const FName& PropertyName)
{
	SaveConfig();

	uint32 PropagationFlags = 0u;

#if ENGINE_MAJOR_VERSION >= 5
	PropagationFlags = UE::ELoadConfigPropagationFlags::LCPF_PropagateToChildDefaultObjects;
#else
	PropagationFlags = UE4::ELoadConfigPropagationFlags::LCPF_PropagateToChildDefaultObjects;
#endif

	ReloadConfig(GetClass(), *GetDefaultConfigFilename(), PropagationFlags, GetClass()->FindPropertyByName(PropertyName));
}

void UHttpGPTSettings::ToggleInternalLogs()
{
#if !UE_BUILD_SHIPPING
	LogHttpGPT_Internal.SetVerbosity(bEnableInternalLogs ? ELogVerbosity::Display : ELogVerbosity::NoLogging);
#endif
}
