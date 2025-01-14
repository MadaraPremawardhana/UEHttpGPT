// Author: Lucas Vilas-Boas
// Year: 2023
// Repo: https://github.com/lucoiso/UEHttpGPT

#pragma once

#include <CoreMinimal.h>
#include <HttpGPTTypes.h>
#include <Widgets/Input/SEditableTextBox.h>
#include <Widgets/Input/STextComboBox.h>
#include <Widgets/Layout/SScrollBox.h>
#include "SHttpGPTChatView.generated.h"

UCLASS(MinimalAPI, NotBlueprintable, NotPlaceable, Category = "Implementation")
class UHttpGPTMessagingHandler : public UObject
{
	GENERATED_BODY()

public:
	explicit UHttpGPTMessagingHandler(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION()
	void RequestSent();

	UFUNCTION()
	void RequestFailed();

	UFUNCTION()
	void ProcessUpdated(const FHttpGPTResponse& Response);
	
	UFUNCTION()
	void ProcessCompleted(const FHttpGPTResponse& Response);

	FHttpGPTMessage Message;

	TSharedPtr<SScrollBox> ScrollBoxReference;

private:
	void ProcessResponse(const FHttpGPTResponse& Response);
};

typedef UHttpGPTMessagingHandler* UHttpGPTMessagingHandlerPtr;

class SHttpGPTChatItem final : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SHttpGPTChatItem) : _MessageRole(), _InputText()
	{
	}
	SLATE_ARGUMENT(EHttpGPTRole, MessageRole)
	SLATE_ARGUMENT(FString, InputText)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	FText GetMessageText() const;

	UHttpGPTMessagingHandlerPtr MessagingHandlerObject;

private:
	TSharedPtr<STextBlock> MessageBox;
};

typedef TSharedPtr<SHttpGPTChatItem> SHttpGPTChatItemPtr;

class SHttpGPTChatView final : public SCompoundWidget
{
public:
	SLATE_USER_ARGS(SHttpGPTChatView)
	{
	}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	FReply HandleSendMessageButton();
	bool IsSendMessageEnabled() const;

	FReply HandleClearChatButton();
	bool IsClearChatEnabled() const;

protected:
	TArray<FHttpGPTMessage> GetChatHistory() const;
	FString GetSystemContext() const;

	void InitializeModelsOptions();

private:
	TSharedPtr<SVerticalBox> ChatBox;
	TArray<SHttpGPTChatItemPtr> ChatItems;

	TSharedPtr<SScrollBox> ChatScrollBox;

	TSharedPtr<SEditableTextBox> InputTextBox;
	TSharedPtr<STextComboBox> ModelsComboBox;

	TArray<TSharedPtr<FString>> AvailableModels;

#if ENGINE_MAJOR_VERSION >= 5
	TObjectPtr<class UHttpGPTRequest> RequestReference;
#else
	class UHttpGPTRequest* RequestReference;
#endif
};
