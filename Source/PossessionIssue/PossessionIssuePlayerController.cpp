// MIT License
//
// Copyright (c) 2023 Guy Elsmore-Paddock
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
#include "PossessionIssuePlayerController.h"

#include <AbilitySystemComponent.h>
#include <AbilitySystemInterface.h>

#include "PossessionIssueCharacterBase.h"

APossessionIssuePlayerController::APossessionIssuePlayerController()
{
	this->PrimaryActorTick.bCanEverTick = true;
}

void APossessionIssuePlayerController::SetPawn(APawn* InPawn)
{
	APossessionIssueCharacterBase*       OldCharacter = Cast<APossessionIssueCharacterBase>(this->GetPawn());
	const APossessionIssueCharacterBase* NewCharacter = Cast<APossessionIssueCharacterBase>(InPawn);

	Super::SetPawn(InPawn);

	if ((OldCharacter != nullptr) && (OldCharacter != NewCharacter))
	{
		const UWorld*  World        = this->GetWorld();
		FTimerManager& TimerManager = World->GetTimerManager();

		// Schedule the old character to properly update their ASC ownership during the next frame. We have to wait for
		// the new controller to replicate.
		TimerManager.SetTimerForNextTick(OldCharacter, &APossessionIssueCharacterBase::RefreshAbilityActorInfo);
	}
}

void APossessionIssuePlayerController::AcknowledgePossession(APawn* P)
{
	const IAbilitySystemInterface* CharacterAscIntf = Cast<IAbilitySystemInterface>(P);

	Super::AcknowledgePossession(P);

	if (CharacterAscIntf != nullptr)
	{
		CharacterAscIntf->GetAbilitySystemComponent()->InitAbilityActorInfo(P, P);
	}
}
