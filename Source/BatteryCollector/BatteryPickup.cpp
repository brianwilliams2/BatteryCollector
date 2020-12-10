// Fill out your copyright notice in the Description page of Project Settings.


#include "BatteryPickup.h"
#include "Pickup.h"

ABatteryPickup::ABatteryPickup()
{
	GetMesh()->SetSimulatePhysics(true);
	BatteryPower = 200.f;
}

void ABatteryPickup::WasCollected_Implementation()
{
	Super::WasCollected_Implementation();
	Destroy();
}

float ABatteryPickup::GetPower()
{
	return BatteryPower;
}

