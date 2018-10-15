// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectPurple.h"
#include "CardData.h"

static TCHAR __CardData[{size}] = {{data}};

int32  CardData::Size = {size};
TCHAR* CardData::Data = reinterpret_cast<TCHAR*>(__CardData);