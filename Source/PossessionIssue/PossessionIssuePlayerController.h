#pragma once

#include <GameFramework/PlayerController.h>

#include "PossessionIssuePlayerController.generated.h"

UCLASS()
class POSSESSIONISSUE_API APossessionIssuePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	APossessionIssuePlayerController();

	virtual void AcknowledgePossession(APawn* P) override;
};
