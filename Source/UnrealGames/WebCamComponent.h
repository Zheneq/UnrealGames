// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"	
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include "WebCamComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FUGWebCamEventDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREALGAMES_API UWebCamComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWebCamComponent();

	// deallocates memory for the opencv fields
	~UWebCamComponent();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called when the component is about to be destroyed
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	// The device ID opened by the Video Stream
	UPROPERTY(EditDefaultsOnly, Category = Webcam)
		int32 CameraID;

	// If the webcam images should be resized every frame
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Webcam)
		bool ShouldResize;

	// The targeted resize width and height (width, height)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Webcam)
		FVector2D ResizeDeminsions;

	// The rate at which the color data array and video texture is updated (in frames per second)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Webcam)
		float RefreshRate;

	// The refresh timer
	UPROPERTY(BlueprintReadOnly, Category = Webcam)
		float RefreshTimer;

	// OpenCV fields
	cv::Mat* frame;
	cv::VideoCapture* stream;
	cv::Size* size;

	// OpenCV prototypes
	void UpdateFrame();
	void UpdateTexture();

	// If the stream has succesfully opened yet
	UPROPERTY(BlueprintReadOnly, Category = Webcam)
		bool isStreamOpen;

	// The videos width and height (width, height)
	UPROPERTY(BlueprintReadOnly, Category = Webcam)
		FVector2D VideoSize;

	// The current video frame's corresponding texture
	UPROPERTY(BlueprintReadOnly, Category = Webcam)
		UTexture2D* VideoTexture;

	// The current data array
	UPROPERTY(BlueprintReadOnly, Category = Webcam)
		TArray<FColor> Data;

	// Blueprint Event called every time the video frame is updated
	UPROPERTY(BlueprintAssignable, Category = "UnrealGames")
		FUGWebCamEventDelegate OnNextVideoFrame;

protected:

	// Use this function to update the texture rects you want to change:
	// NOTE: There is a method called UpdateTextureRegions in UTexture2D but it is compiled WITH_EDITOR and is not marked as ENGINE_API so it cannot be linked
	// from plugins.
	// FROM: https://wiki.unrealengine.com/Dynamic_Textures
	void UpdateTextureRegions(UTexture2D* Texture, int32 MipIndex, uint32 NumRegions, FUpdateTextureRegion2D* Regions, uint32 SrcPitch, uint32 SrcBpp, uint8* SrcData, bool bFreeData);

	// Pointer to update texture region 2D struct
	FUpdateTextureRegion2D* VideoUpdateTextureRegion;
};
