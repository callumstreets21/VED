// Fill out your copyright notice in the Description page of Project Settings.


#include "STickProfilingWindow.h"



TSharedPtr<SListView<TSharedPtr<FClassItem>>> ClassListView = nullptr;
TArray<TSharedPtr<FClassItem>> ClassItems;

void STickProfilingWindow::Construct(const FArguments& InArgs)
{
    UpdateCoreProfilingSubsystem();
    
    FClassViewerInitializationOptions InitOptions;
    InitOptions.bShowUnloadedBlueprints = true;
    InitOptions.ClassFilters.Add(MakeShareable(new FActorClassFilter()));

    // Get the Class Viewer module
    FClassViewerModule& ClassViewerModule = FModuleManager::LoadModuleChecked<FClassViewerModule>("ClassViewer");

    TSharedRef<SWidget> ClassViewerWidget = ClassViewerModule.CreateClassViewer(
        InitOptions,
        FOnClassPicked::CreateLambda([this](UClass* InClass) -> void
            {
                this->OnClassSelected(InClass);
            })
    );

    ChildSlot
        [
            SNew(SExpandableArea)
            .AreaTitle(FText::FromString("Class Blacklist"))
            .InitiallyCollapsed(false)
            .BodyContent()
            [
            SNew(SVerticalBox)
                
            + SVerticalBox::Slot()
            .MaxHeight(200.f) 
            .Padding(5)
            [
                GraphWidget
            ]
            + SVerticalBox::Slot()
            .AutoHeight()
            .Padding(5)
            [
                SNew(SHorizontalBox)
                + SHorizontalBox::Slot()
                .AutoWidth()
                [
                    SNew(STextBlock)
                    .Text(FText::FromString("Longest Tick Last Frame:"))
                ]
                + SHorizontalBox::Slot()
                .AutoWidth()
                .Padding(5, 0)
                [
                    SAssignNew(LongestTickTextBlock, STextBlock)
                    .Text_Lambda([this]() -> FText {
                        UpdateCoreProfilingSubsystem();
                        if (CoreProfilingSubsystem.IsValid())
                        {
                            float LongestTick = CoreProfilingSubsystem->GetLongestTickLastFrame();
                            
                            return FText::FromString(FString::Printf(TEXT("%.4f ms"), LongestTick * 1000.0f)); // Convert to milliseconds
                        }
                        return FText::FromString("N/A");
                        })
                ]
            ]
            + SVerticalBox::Slot()
            .AutoHeight()
            .Padding(5)
            [
                SNew(SHorizontalBox)
                + SHorizontalBox::Slot()
                .AutoWidth()
                [
                    SNew(STextBlock)
                    .Text(FText::FromString("Longest Tick Actor Name:"))
                ]
                + SHorizontalBox::Slot()
                .AutoWidth()
                .Padding(5, 0)
                [
                    SAssignNew(LongestTickNameTextBlock, STextBlock)
                    .Text_Lambda([this]() -> FText {
                        UpdateCoreProfilingSubsystem();
                        if (CoreProfilingSubsystem.IsValid())
                        {
                            FString name = CoreProfilingSubsystem->GetLongestTickActorName();
                            return FText::FromString(name); // Convert to milliseconds
                        }
                        return FText::FromString("N/A");
                        })
                ]
            ]    
            // Add checkbox for visualization toggle
            + SVerticalBox::Slot()
            .AutoHeight()
            .Padding(5)
            [
                SNew(SHorizontalBox)
                + SHorizontalBox::Slot()
                .AutoWidth()
                [
                    SNew(STextBlock)
                    .Text(FText::FromString("Enable Visualization"))
                ]
                + SHorizontalBox::Slot()
                .AutoWidth()
                .Padding(5, 0)
                [
                    SNew(SCheckBox)
                    .OnCheckStateChanged_Lambda([this](ECheckBoxState NewState) {
                        UpdateCoreProfilingSubsystem();
                        if (CoreProfilingSubsystem.IsValid())
                        {
                            CoreProfilingSubsystem->SetEnableVisualization(NewState == ECheckBoxState::Checked);
                        }
                    })
                ]
            ]
            // Add checkbox to disable profiling
            + SVerticalBox::Slot()
            .AutoHeight()
            .Padding(5)
            [
                SNew(SHorizontalBox)
                + SHorizontalBox::Slot()
                .AutoWidth()
                [
                    SNew(STextBlock)
                        .Text(FText::FromString("Enable Profiling"))
                ]
                + SHorizontalBox::Slot()
                .AutoWidth()
                .Padding(5, 0)
                [
                    SNew(SCheckBox)
                    .OnCheckStateChanged_Lambda([this](ECheckBoxState NewState) {
                        UpdateCoreProfilingSubsystem();
                        if (CoreProfilingSubsystem.IsValid())
                        {
                            CoreProfilingSubsystem->SetEnableProfiling(NewState == ECheckBoxState::Checked);
                        }
                    })
                ]
            ]
            // The top bar: label and Add button
            + SVerticalBox::Slot()
            .AutoHeight()
            .Padding(5)
            [

                SNew(SHorizontalBox)
                + SHorizontalBox::Slot()
                .AutoWidth()
                [
                    SNew(STextBlock)
                    .Text(FText::FromString("Blacklist Items"))
                ]
                + SHorizontalBox::Slot()
                .AutoWidth()
                .Padding(5,0)
                [
                    
                    SNew(SButton)
                    .Text(FText::FromString("Add"))
                    .OnClicked(this, &STickProfilingWindow::OnAddButtonClicked)
                    
                ]
            ]
            // Container for rows
            + SVerticalBox::Slot()
            .FillHeight(1.0f)
            [
                SAssignNew(BlacklistVBox, SVerticalBox)
            ]
        ]
    ];
    RebuildBlacklistRows();
}  




TSharedRef<ITableRow> STickProfilingWindow::GenerateClassRow(TSharedPtr<FClassItem> _Item, const TSharedRef<STableViewBase>& OwnerTable)
{
    // Generate each row for the SListView
    return SNew(STableRow<TSharedPtr<UClass>>, OwnerTable)
    [
        SNew(SHorizontalBox)
        + SHorizontalBox::Slot()
        .Padding(5)
        [
            SNew(STextBlock)
            .Text(_Item->ClassName)  // Display the class name in the row
        ]
        + SHorizontalBox::Slot()
        .Padding(5)
        [
            SNew(SButton)
            .Text(FText::FromString(TEXT("Select")))
            .OnClicked_Lambda([this, _Item]() -> FReply {
                return OnClassSelected(_Item->Class);  // Pass the selected class to the handler
            })
        ]
    ];
}

TArray<UClass*> STickProfilingWindow::GetAllActorClasses()
{
    TArray<UClass*> ActorClasses;
    
    FAssetRegistryModule& AssetRegistryModule = FModuleManager::GetModuleChecked<FAssetRegistryModule>("AssetRegistry");
    IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();
    
    // Use the blueprint path instead; the UClass assets for Blueprints use "Blueprint" as the class name.
    TArray<FAssetData> BlueprintAssets;
    FTopLevelAssetPath BlueprintClassPath = FTopLevelAssetPath(TEXT("/Script/Engine.Blueprint"));
    FARFilter Filter;
    // You can filter by package path if you wish, e.g. "/Game"
    Filter.ClassPaths.Add(BlueprintClassPath);
    AssetRegistry.GetAssets(Filter, BlueprintAssets);
    
    // For each Blueprint asset, attempt to load the generated class.
    for (const FAssetData& AssetData : BlueprintAssets)
    {
        // The asset should have a "GeneratedClass" tag we can look up.
        FString GeneratedClassPath;
        if (AssetData.GetTagValue("GeneratedClass", GeneratedClassPath))
        {
            // Convert the path to a UClass*
            UClass* BlueprintClass = LoadObject<UClass>(nullptr, *GeneratedClassPath);
            if (BlueprintClass && BlueprintClass->IsChildOf(AActor::StaticClass()))
            {
                ActorClasses.Add(BlueprintClass);
            }
        }
    }
    
    UE_LOG(LogTemp, Warning, TEXT("Found %d blueprint actor classes"), ActorClasses.Num());
    return ActorClasses;
}


FReply STickProfilingWindow::OnClassSelected(UClass* InClass)
{
    // Handle the class selection (e.g., add to the blacklist, show details, etc.)
    UE_LOG(LogTemp, Warning, TEXT("Selected class: %s"), *InClass->GetName());
    return FReply::Handled();
}

FReply STickProfilingWindow::OnAddButtonClicked()
{
    UpdateCoreProfilingSubsystem();
    CoreProfilingSubsystem->GetBlacklist().Add(nullptr);
    RebuildBlacklistRows();  
    return FReply::Handled();
}

TSharedRef<SWidget> STickProfilingWindow::GenerateBlacklistRow(int32 RowIndex)
{
    UpdateCoreProfilingSubsystem();
    // Get the current class from the subsystem's blacklist.
    UClass* BlacklistedClass = (CoreProfilingSubsystem->GetBlacklist().IsValidIndex(RowIndex)) ? CoreProfilingSubsystem->GetBlacklist()[RowIndex] : nullptr;
    FText ClassName = BlacklistedClass ? FText::FromString(BlacklistedClass->GetName()) : FText::FromString("None");
    
    return SNew(SHorizontalBox)
    // Display the class name.
    + SHorizontalBox::Slot()
    .FillWidth(1.0f)
    .Padding(5)
    [
        SNew(STextBlock)
        .Text(ClassName)
    ]
    // "Select" button to open the class selector popup.
    + SHorizontalBox::Slot()
    .AutoWidth()
    .Padding(5)
    [
        SNew(SButton)
        .Text(FText::FromString("Select"))
        .OnClicked_Lambda([this, RowIndex]() -> FReply 
        {
            return OnOpenClassSelectorClicked(RowIndex);
        })
    ]
    // "Remove" button to remove the entry from the blacklist.
    + SHorizontalBox::Slot()
    .AutoWidth()
    .Padding(5)
    [
        SNew(SButton)
        .Text(FText::FromString("Remove"))
        .OnClicked_Lambda([this, RowIndex]() -> FReply
        {
            UpdateCoreProfilingSubsystem();
            if (CoreProfilingSubsystem.IsValid() && CoreProfilingSubsystem->GetBlacklist().IsValidIndex(RowIndex))
            {
                // Remove the item at RowIndex.
                CoreProfilingSubsystem->GetBlacklist().RemoveAt(RowIndex);
                RebuildBlacklistRows();
            }
            return FReply::Handled();
        })
    ];
}
void STickProfilingWindow::UpdateCoreProfilingSubsystem()
{
    UWorld* World = GEditor ? GEditor->GetEditorWorldContext().World() : nullptr;

    UWorld* PIEWorld = GetPIEWorld();
    if (PIEWorld)
    {
        World = PIEWorld;
    }

    if (World)
    {
        CoreProfilingSubsystem = World->GetSubsystem<UCoreProfilingSubsystem>();
    }
}

void STickProfilingWindow::RebuildBlacklistRows()
{

    UpdateCoreProfilingSubsystem();

    
    if (BlacklistVBox.IsValid())
    {
        BlacklistVBox->ClearChildren();

        // For each entry in the Blacklist array, add a row.
        for (int32 Index = 0; Index < CoreProfilingSubsystem->GetBlacklist().Num(); ++Index)
        {
            BlacklistVBox->AddSlot()
            .AutoHeight()
            .Padding(2)
            [
                GenerateBlacklistRow(Index)
            ];
        }
        UE_LOG(LogTemp, Warning, TEXT("Blacklist rebuilt, Calling Reinjection"));
        CoreProfilingSubsystem->ReinjectProfilingComponents();
    }
}

FReply STickProfilingWindow::OnOpenClassSelectorClicked(int32 RowIndex)
{
    UpdateCoreProfilingSubsystem();
    // Create a new window (popup) that will host the class viewer.
    TSharedRef<SWindow> PickerWindow = SNew(SWindow)
        .Title(FText::FromString("Select Class"))
        .ClientSize(FVector2D(400, 600))
        .SupportsMinimize(false)
        .SupportsMaximize(false);

    // Set up the class viewer initialization options.
    FClassViewerInitializationOptions InitOptions;
    InitOptions.bShowUnloadedBlueprints = true;
    InitOptions.ClassFilters.Add(MakeShareable(new FActorClassFilter()));
	
    // Load the Class Viewer module.
    FClassViewerModule& ClassViewerModule = FModuleManager::LoadModuleChecked<FClassViewerModule>("ClassViewer");
	
    // Create the class viewer widget with a lambda callback.
    TSharedRef<SWidget> ClassViewerWidget = ClassViewerModule.CreateClassViewer(
        InitOptions,
        FOnClassPicked::CreateLambda([this, RowIndex, PickerWindow](UClass* InClass)
        {
            this->OnClassSelected(InClass, RowIndex);
            CoreProfilingSubsystem->ReinjectProfilingComponents();
            // Close the window after selection.
            FSlateApplication::Get().RequestDestroyWindow(PickerWindow);
        })
    );
	
    // Set the content of the window.
    PickerWindow->SetContent(ClassViewerWidget);
    FSlateApplication::Get().AddWindow(PickerWindow);
	
    return FReply::Handled();
}

void STickProfilingWindow::OnClassSelected(UClass* InClass, int32 RowIndex)
{
	UpdateCoreProfilingSubsystem();
    if (CoreProfilingSubsystem->GetBlacklist().IsValidIndex(RowIndex))
    {
        CoreProfilingSubsystem->GetBlacklist()[RowIndex] = InClass;
        RebuildBlacklistRows();
        UE_LOG(LogTemp, Warning, TEXT("Row %d updated to class: %s"), RowIndex, *InClass->GetName());
    }
}