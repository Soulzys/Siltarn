#include "Weapons/DebugActor.h"

DEFINE_LOG_CATEGORY(LogClass_DebugActor);
DEFINE_LOG_CATEGORY(LogStruct_FDebugActore);

int32 ADebugActor::m_InstanceCount = 0;

ADebugActor::ADebugActor()
{
	UE_LOG(LogClass_DebugActor, Log, TEXT("Constructor called !"));
	m_InstanceCount++;
	UE_LOG(LogClass_DebugActor, Log, TEXT("Instance count : %d !"), m_InstanceCount);

	PrimaryActorTick.bCanEverTick = false;
}

ADebugActor::~ADebugActor()
{
	UE_LOG(LogClass_DebugActor, Log, TEXT("Destructor called !"));
	m_InstanceCount--;
	UE_LOG(LogClass_DebugActor, Log, TEXT("Instance count : %d !"), m_InstanceCount);
}

void ADebugActor::BeginPlay()
{
	Super::BeginPlay();
}







int32 FDebugActore::s_InstanceCount = 0;

FDebugActore::FDebugActore()
{
	UE_LOG(LogStruct_FDebugActore, Log, TEXT("Constructor called !"));
	s_InstanceCount++;
	UE_LOG(LogStruct_FDebugActore, Log, TEXT("Instance count : %d"), s_InstanceCount);
}

FDebugActore::~FDebugActore()
{
	UE_LOG(LogStruct_FDebugActore, Log, TEXT("Destructor called !"));
	s_InstanceCount--;
	UE_LOG(LogStruct_FDebugActore, Log, TEXT("Instance count : %d"), s_InstanceCount);
}