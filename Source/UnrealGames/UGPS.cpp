// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealGames.h"
#include "UGPS.h"

#include "Online.h"
#include "ThirdParty/Steamworks/Steamv132/sdk/public/steam/steam_api.h"
#include "Runtime/Online/OnlineSubsystem/Public/OnlineSubsystem.h"
#include "Net/UnrealNetwork.h"



void AUGPS::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const

{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AUGPS, bMyTurn);
	DOREPLIFETIME(AUGPS, bIsInGame);
	DOREPLIFETIME(AUGPS, UGScore);
	DOREPLIFETIME(AUGPS, PlayerIndex);
	DOREPLIFETIME(AUGPS, PlayerColor);
	DOREPLIFETIME(AUGPS, Next);

}

UTexture2D* AUGPS::GetSteamAvatar(int32 &width, int32 &height)
{
	uint32 Width;
	uint32 Height;
	width = height = 0;

	if (IOnlineSubsystem::Get(FName("Steam")) && SteamAPI_Init())
	{
		//Getting the PictureID from the SteamAPI and getting the Size with the ID
		int Picture = SteamFriends()->GetLargeFriendAvatar(CSteamID((uint64)FCString::Atoi64(*UniqueId->ToString())));
		SteamUtils()->GetImageSize(Picture, &Width, &Height);


		if (Width > 0 && Height > 0)
		{
			//Creating the buffer "oAvatarRGBA" and then filling it with the RGBA Stream from the Steam Avatar
			BYTE *oAvatarRGBA = new BYTE[Width * Height * 4];


			//Filling the buffer with the RGBA Stream from the Steam Avatar and creating a UTexture2D to parse the RGBA Steam in
			SteamUtils()->GetImageRGBA(Picture, (uint8*)oAvatarRGBA, 4 * Height * Width * sizeof(char));

			//Swap R and B channels because for some reason the games whack
			for (uint32 i = 0; i < (Width * Height * 4); i += 4)
			{
				uint8 Temp = oAvatarRGBA[i + 0];
				oAvatarRGBA[i + 0] = oAvatarRGBA[i + 2];
				oAvatarRGBA[i + 2] = Temp;
			}

			// Creating a texture and copying data
			UTexture2D* Avatar = UTexture2D::CreateTransient(Width, Height, PF_B8G8R8A8);

			uint8* MipData = (uint8*)Avatar->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
			FMemory::Memcpy(MipData, (void*)oAvatarRGBA, Height * Width * 4);
			Avatar->PlatformData->Mips[0].BulkData.Unlock();

			//Setting some Parameters for the Texture and finally returning it
			Avatar->PlatformData->NumSlices = 1;
			Avatar->NeverStream = true;
			//Avatar->CompressionSettings = TC_EditorIcon;

			Avatar->UpdateResource();

			width = (int32)Width;
			height = (int32)Height;
			return Avatar;
		}
		UE_LOG(LogTemp, Log, TEXT("UGPS::GetAvatar: Steam does not provide an avatar."));
		return nullptr;
	}
	UE_LOG(LogTemp, Log, TEXT("UGPS::GetAvatar: Steam is unavailable."));
	return nullptr;
}

