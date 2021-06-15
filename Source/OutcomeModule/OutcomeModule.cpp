#include "OutcomeModule.h"

IMPLEMENT_MODULE(FOutcomeModule, OutcomeModule);

void FOutcomeModule::StartupModule()
{
	UE_LOG( LogTemp, Warning, TEXT("Outcome Module has started!" ) );
}

void FOutcomeModule::ShutdownModule()
{
    // Put your module termination code here
}