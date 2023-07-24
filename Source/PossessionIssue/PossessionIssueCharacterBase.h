#pragma once

#include <AbilitySystemInterface.h>

#include <GameFramework/Character.h>

#include "PossessionIssueCharacterBase.generated.h"

// =====================================================================================================================
// Forward Declarations (to minimize header dependencies)
// =====================================================================================================================

// =====================================================================================================================
// Normal Declarations
// =====================================================================================================================
UCLASS(Abstract)
class POSSESSIONISSUE_API APossessionIssueCharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

protected:
	// =================================================================================================================
	// Protected Fields
	// =================================================================================================================
	/**
	 * The Ability System Component (ASC) used for interfacing this character with the Gameplay Abilities System (GAS).
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UAbilitySystemComponent* AbilitySystemComponent;

public:
	// =================================================================================================================
	// Public Constructors
	// =================================================================================================================
	/**
	 * Default constructor.
	 */
	explicit APossessionIssueCharacterBase();

	// =================================================================================================================
	// Public Methods - IAbilitySystemInterface Implementation
	// =================================================================================================================
	/**
	 * Gets the UObject version of the Ability System Component (ASC) for this character.
	 *
	 * @return
	 *	The ASC for this character.
	 */
	UFUNCTION(BlueprintCallable)
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	// =================================================================================================================
	// Public Methods
	// =================================================================================================================
	virtual void PossessedBy(AController * NewController) override;

};
