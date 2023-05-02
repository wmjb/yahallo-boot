// DeviceLut.c: Device Look-up table

#include "Include/Application.h"
#include "Include/Devices.h"

// Hack version LUT
VERSION_TABLE_ENTRY gVersionEntries[255] = {
    {
        // Surface RT v3.31.500, Tegra 3, needs firmware fix-up
        // gST string "OemkS EFI Jan 24 2014 18:00:42"
        L"OemkS EFI Jan 24 2014 18:00:42",
        SurfaceRTExploit,
        Tegra3ConsoleOutputFixup,
    },
    {
        // YOGA 11 test, Tegra 3, needs firmware fix-up
        // gST string "VenusTZ EFI Sep 28 2012 04:37:26"
        L"Venus EFI Dec  5 2012 23:09:45",
        Yoga_11_Exploit,
        Tegra3ConsoleOutputFixup,
    },
        {
        // YOGA 11 test, Tegra 3, needs firmware fix-up
        // gST string "VenusTZ EFI Sep 28 2012 04:37:26"
        L"Venus EFI Dec 5 2012 23:09:45",
        Yoga_11_Exploit,
        Tegra3ConsoleOutputFixup,
    },
    {
        // VivoTab test, Tegra 3, needs firmware fix-up
        // gST string "VenusTZ EFI Sep 28 2012 04:37:26"
        L"ASUS UEFI Jul 24 2013 18:23:22",
        VivoTab_Exploit,
        Tegra3ConsoleOutputFixup,
    },
    {
        // Surface 2 v2.6.500, Tegra 4
        // gST string "Surface 2 EFI Aug 26 2013 19:20:25"
        L"Covington EFI Aug 26 2013 19:20:25",
        Surface2_Fw_2_06_500_Exploit,
        Tegra3ConsoleOutputFixup,
    },
    {
        // Surface 2 v4.22.500, Tegra 4
        // gST string "Surface 2 EFI Sep 11 2014 00:32:29"
        L"Surface 2 EFI Sep 11 2014 00:32:29",
        Surface2_Fw_4_22_500_Exploit,
        NULL,
    },
    {
        // Terminator
        NULL,
    },
};

EFI_STATUS LaunchExploitByVersionTable(VOID)
{
  EFI_STATUS           Status = EFI_SUCCESS;
  PVERSION_TABLE_ENTRY pEntry = NULL;
    Tegra3ConsoleOutputFixup();
    
  if (gST->FirmwareVendor != NULL) {
    Print(
        L"Your firmware (gST): %s, 0x%x\n", gST->FirmwareVendor,
        gST->FirmwareRevision);
;

  	Print(L"Loading device.inf into memory\n");
	size_t fileSize1 = 0;
	Status = loadPayloadIntoMemory((EFI_PHYSICAL_ADDRESS)0x83000000, L"\\device.inf", &fileSize1);
	if (Status = EFI_SUCCESS)
	{
        //do the thing with the device.inf so it's used as a lut


	}

    //fall back to built in lut
    else
    {
        Print(L"Matching device\n")
        PVERSION_TABLE_ENTRY pLut = (PVERSION_TABLE_ENTRY)&gVersionEntries;

        do {
        if (StrStr(gST->FirmwareVendor, pLut->FirmwareRelease) != NULL) {
            pEntry = pLut;
          break;
          }
          pLut++;
        } while (pLut->FirmwareRelease != NULL);
      }
      else {
        Print(L"[WARN] Failed to read firmware release from EFI System Table\n");
      }


   if (pEntry == NULL) {
    // Fix the console anyway (because we don't know)
    //Tegra3ConsoleOutputFixup(); not needed as done anyway above
    Print(L"Yahallo - Tegra 3/4 Secure Boot Unlock Utility\n");
    Print(L"[ERROR] Failed to find the device. It is probably not supported "
          L"yet\n");
        Print(
            L"Your firmware (gST): %s, 0x%x\n", gST->FirmwareVendor,
            gST->FirmwareRevision);
         // tegra3dumpfirmware(); //disabled as increases file size
        Status = EFI_NOT_FOUND;
        goto exit;
      }
         else {
        // Run pre-fix up if exists
        if (pEntry->PreEntryFixup != NULL) {
          HACK_ENTRY pFixupEntry = (HACK_ENTRY)pEntry->PreEntryFixup;
          pFixupEntry();
        }

            // Go    
        Print(
            L"Your firmware (gST): %s, 0x%x\n", gST->FirmwareVendor,
            gST->FirmwareRevision);

        HACK_ENTRY pHackEntry = (HACK_ENTRY)pEntry->EntryPoint;
        pHackEntry();
      }

    exit:
      return Status;
    }
}
