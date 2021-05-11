#pragma once
#include "HexagonMath.h"
#include "AnemoneHexTile.generated.h"

class AAnemoneDungeonLevel;

UENUM( BlueprintType, meta = ( ScriptName="TileType" ) )
 enum class EAnemoneTileType : uint8
 {
    Navigable   UMETA( DisplayName = "NAVIGABLE" ),
    Interactive UMETA( DisplayName = "INTERACTIVE" )
 };

UCLASS()
class AAnemoneHexTile : public AActor
{
    GENERATED_BODY()
public:
   	UPROPERTY( EditInstanceOnly, BlueprintReadWrite )
	FAxialCoordinates Coordinates;

   	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite )
	EAnemoneTileType TileType;

   	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite )
	float CameraHeightOffset;

	UFUNCTION( BlueprintCallable, BlueprintImplementableEvent )
    void ActivateInteraction();

	bool bExistsInLevel = false;

    virtual void OnConstruction( const FTransform & Transform );
	virtual void Destroyed() override;
private:
#if WITH_EDITOR
    virtual void PostLoad();
    virtual void PostEditMove( bool bFinished ) override;
#endif
    AAnemoneDungeonLevel* FetchDungeonLevel();
};
