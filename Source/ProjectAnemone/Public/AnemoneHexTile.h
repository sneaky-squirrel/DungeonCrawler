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
	UFUNCTION( BlueprintCallable, BlueprintImplementableEvent )
    void ActivateInteraction();

	bool bExistsInLevel = false;

    virtual void OnConstruction( const FTransform & Transform );
	virtual void Destroyed() override;
    void ExtractCoordinates( int32& InQ, int32& InR, int32& InH, int32& InAbsQ, int32& InAbsR, int32& InAbsH ) const;

protected:
   	UPROPERTY( EditInstanceOnly, BlueprintReadWrite )
	FAxialCoordinates Coordinates;

   	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite )
	EAnemoneTileType TileType;

   	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite )
	float CameraHeightOffset;

#if WITH_EDITOR
    virtual void PostLoad();
    virtual void PostEditMove( bool bFinished ) override;
#endif
    AAnemoneDungeonLevel* FetchDungeonLevel();

    friend class AAnemoneDungeonLevel;
};
