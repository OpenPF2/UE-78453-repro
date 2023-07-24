#include "PossessionIssueCharacterBase.h"

#include <AbilitySystemComponent.h>

APossessionIssueCharacterBase::APossessionIssueCharacterBase()
{
	UAbilitySystemComponent* NewAsc =
		this->CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));

	NewAsc->SetIsReplicated(true);

	this->AbilitySystemComponent = NewAsc;
}

UAbilitySystemComponent* APossessionIssueCharacterBase::GetAbilitySystemComponent() const
{
	check(this->AbilitySystemComponent);
	return this->AbilitySystemComponent;
}

void APossessionIssueCharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (this->AbilitySystemComponent != nullptr)
	{
		this->AbilitySystemComponent->InitAbilityActorInfo(this, this);
	}

	// ASC MixedMode replication requires that the ASC Owner's Owner be the Controller.
	this->SetOwner(NewController);
}
