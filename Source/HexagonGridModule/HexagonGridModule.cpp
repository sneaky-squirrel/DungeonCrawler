#include "HexagonGridModule.h"

IMPLEMENT_MODULE(FHexagonGridModule, HexagonGridModule);

void FHexagonGridModule::StartupModule()
{
	UE_LOG( LogTemp, Warning, TEXT("HexagonGrid Module has started!" ) );
}

void FHexagonGridModule::ShutdownModule()
{
    // Put your module termination code here
}