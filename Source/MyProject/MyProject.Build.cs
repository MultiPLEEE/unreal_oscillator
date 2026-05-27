// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MyProject : ModuleRules
{
    public MyProject(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
    
        PublicDependencyModuleNames.AddRange(new string[] {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "Slate",
            "SlateCore",
            "UMG",
            "RenderCore"      // Добавлено для FSlateDrawElement
        });
        
        PrivateDependencyModuleNames.AddRange(new string[] {
            "Slate",          // Для FSlateApplication
            "SlateCore",      // Для FSlateFontMeasure
            "RenderCore"      // Для отрисовки
        });
        
        // Разрешаем использование непубличных заголовков Slate
        PrivateIncludePathModuleNames.AddRange(new string[] {
            "Slate",
            "SlateCore"
        });
    }
}
