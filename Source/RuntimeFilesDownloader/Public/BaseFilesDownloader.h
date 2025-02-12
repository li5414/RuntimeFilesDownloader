// Georgy Treshchev 2022.

#pragma once

#include "Http.h"

#include "BaseFilesDownloader.generated.h"

/** Dynamic delegate to track download progress */
DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnDownloadProgress, int32, BytesReceived, int32, ContentLength);

/** Static delegate to track download progress */
DECLARE_DELEGATE_TwoParams(FOnDownloadProgressNative, int32, int32);


/**
 * Base class for downloading files. It also contains some helper functions
 */
UCLASS(BlueprintType, Category = "Runtime Files Downloader")
class RUNTIMEFILESDOWNLOADER_API UBaseFilesDownloader : public UObject
{
	GENERATED_BODY()

protected:

	/** Static delegate to track download progress */
	FOnDownloadProgressNative OnDownloadProgressNative;

	/** Dynamic delegate to track download progress */
	FOnDownloadProgress OnDownloadProgress;

public:
	/**
	 * File downloading progress internal callback
	 */
	void OnProgress_Internal(FHttpRequestPtr Request, int32 BytesSent, int32 BytesReceived) const;

	/**
	 * Canceling the current download
	 *
	 * @return Whether the cancellation was successful or not
	 */
	UFUNCTION(BlueprintCallable, Category = "Runtime Files Downloader|Main")
	bool CancelDownload();

	/**
	 * Convert bytes to string
	 */
	UFUNCTION(BlueprintCallable, Category = "Runtime Files Downloader|Utilities")
	static FString BytesToString(const TArray<uint8>& Bytes);

	/**
	 * Convert bytes to texture
	 */
	UFUNCTION(BlueprintCallable, Category = "Runtime Files Downloader|Utilities")
	static UTexture2D* BytesToTexture(const TArray<uint8>& Bytes);

	/**
	 * Load a binary file to a dynamic array with two uninitialized bytes at end as padding
	 */
	UFUNCTION(BlueprintCallable, Category = "Runtime Files Downloader|Utilities")
	static bool LoadFileToArray(TArray<uint8>& Result, const FString& Filename);

	/**
	 * Save a binary array to a file
	 */
	UFUNCTION(BlueprintCallable, Category = "Runtime Files Downloader|Utilities")
	static bool SaveArrayToFile(const TArray<uint8>& Bytes, const FString& Filename);

	/**
	 * Load a text file to an FString.
	 * Supports all combination of ANSI/Unicode files and platforms
	 * 
	 * @param Result string representation of the loaded file
	 * @param Filename name of the file to load
	 */
	UFUNCTION(BlueprintCallable, Category = "Runtime Files Downloader|Utilities")
	static bool LoadFileToString(FString& Result, const FString& Filename);

	/**
	 * Write the string to a file.
	 * Supports all combination of ANSI/Unicode files and platforms
	 */
	UFUNCTION(BlueprintCallable, Category = "Runtime Files Downloader|Utilities")
	static bool SaveStringToFile(const FString& String, const FString& Filename);

protected:
	/** Http download request */
	IHttpRequest* HttpDownloadRequest;

	/**
	 * Broadcast the progress both multi-cast and single-cast delegates
	 * @note To get the download percentage, divide the BytesReceived value by the ContentLength
	 */
	void BroadcastProgress(const int32 BytesReceived, const int32 ContentLength) const;
};
