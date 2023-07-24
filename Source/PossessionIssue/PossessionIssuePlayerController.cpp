#include "PossessionIssuePlayerController.h"

#include <AbilitySystemComponent.h>
#include <AbilitySystemInterface.h>

APossessionIssuePlayerController::APossessionIssuePlayerController()
{
	this->PrimaryActorTick.bCanEverTick = true;
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
