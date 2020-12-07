#pragma once

#include "GameFramework/Pawn.h"
#include "FGPlayer.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UFGMovementComponent;
class UStaticMeshComponent;
class USphereComponent;
class UFGPlayerSettings;
class UFG19_NetworkingDebugWidget;

UCLASS()
class FG19_NETWORKING_API AFGPlayer : public APawn
{
	GENERATED_BODY()

public:
	AFGPlayer();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, Category = Settings)
	UFGPlayerSettings* PlayerSettings = nullptr;

	UFUNCTION(BlueprintPure)
	bool IsBraking() const { return bBrake; }

	UFUNCTION(BlueprintPure)
	int32 GetPing() const;
	
	UPROPERTY(EditAnywhere, Category = Debug)
	TSubclassOf<UFG19_NetworkingDebugWidget> DebugMenuClass;

	UFUNCTION(Server, Unreliable)
	void Server_SendLocationAndRotation(const FVector& LocationToSend, const FRotator& RotationToSend);

	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_SendLocationAndRotation(const FVector& LocationToSend, const FRotator& RotationToSend);

	void ShowDebugMenu();
	void HideDebugMenu();

private:
	FVector OldLocation;
	FRotator OldRotation;

	FVector NewLocation;
	FRotator NewRotation;

	float InterpolationAlpha = 4.0f;

	void Handle_Accelerate(float Value);
	void Handle_Turn(float Value);
	void Handle_BrakePressed();
	void Handle_BrakeReleased();

	void Handle_DebugMenuPressed();
	void CreateDebugWidget();

	UPROPERTY(Transient)
	UFG19_NetworkingDebugWidget* DebugMenuInstance = nullptr;

	bool bShowDebugMenu = false;

	float Forward = 0.0f;
	float Turn = 0.0f;

	float MovementVelocity = 0.0f;
	float Yaw = 0.0f;

	bool bBrake = false;

	UPROPERTY(VisibleDefaultsOnly, Category = Collision)
	USphereComponent* CollisionComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = Camera)
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = Camera)
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = Movement)
	UFGMovementComponent* MovementComponent;
};