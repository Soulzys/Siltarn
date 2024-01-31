#include "Interactables/ItemBag.h"

void AItemBag::BeginPlay()
{
	Super::BeginPlay();

	m_StaticMesh->AddImpulse(GetActorForwardVector() * 250000.0f);
}