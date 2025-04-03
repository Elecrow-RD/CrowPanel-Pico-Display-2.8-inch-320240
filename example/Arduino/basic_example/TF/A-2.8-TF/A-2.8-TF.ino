#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <FS.h>

File root;
const int _MISO = 12;  // AKA SPI RX
const int _MOSI = 11;  // AKA SPI TX
const int _CS = 22;
const int _SCK = 10;
void setup() {
  // put your setup code here, to run once:
  Serial.begin( 9600 ); /*初始化串口*/
  //SD卡
  // SPI.begin(SD_SCK, SD_MISO, SD_MOSI);
  // delay(100);
  // if (SD_init() == 1)
  // {
  //   Serial.println("Card Mount Failed");
  // }
  // else
  //   Serial.println("initialize SD Card successfully");
}

void loop() {
  Serial1.println("\nInitializing SD card...");
  Serial.println("\nInitializing SD card...");
  bool sdInitialized = false;
  // Ensure the SPI pinout the SD card is connected to is configured properly
  // Select the correct SPI based on _MISO pin for the RP2040
  if (_MISO == 0 || _MISO == 4 || _MISO == 16) {
    SPI.setRX(_MISO);
    SPI.setTX(_MOSI);
    SPI.setSCK(_SCK);
    sdInitialized = SD.begin(_CS);
  } else if (_MISO == 8 || _MISO == 12) {
    SPI1.setRX(_MISO);
    SPI1.setTX(_MOSI);
    SPI1.setSCK(_SCK);
    sdInitialized = SD.begin(_CS, SPI1);
  } else {
    Serial.println(F("ERROR: Unknown SPI Configuration"));
    Serial1.println(F("ERROR: Unknown SPI Configuration"));
    return;
  }

  if (!sdInitialized) {
    Serial1.println("initialization failed. Things to check:");
    Serial1.println("* is a card inserted?");
    Serial1.println("* is your wiring correct?");
    Serial1.println("* did you change the chipSelect pin to match your shield or module?");
    
    Serial.println("initialization failed. Things to check:");
    Serial.println("* is a card inserted?");
    Serial.println("* is your wiring correct?");
    Serial.println("* did you change the chipSelect pin to match your shield or module?");
    return;
  } else {
    Serial1.println("Wiring is correct and a card is present.");
    Serial.println("Wiring is correct and a card is present.");
  }
  // 0 - SD V1, 1 - SD V2, or 3 - SDHC/SDXC
  // print the type of card
  Serial1.println();
  Serial1.print("Card type:         ");
  Serial.println();
  Serial.print("Card type:         ");
  switch (SD.type()) {
    case 0:
      Serial1.println("SD1");
      Serial.println("SD1");
      break;
    case 1:
      Serial1.println("SD2");
      Serial.println("SD2");
      break;
    case 3:
      Serial1.println("SDHC/SDXC");
      Serial.println("SDHC/SDXC");
      break;
    default:
      Serial1.println("Unknown");
      Serial.println("Unknown");
  }

  Serial1.print("Cluster size:          ");
  Serial1.println(SD.clusterSize());
  Serial1.print("Blocks x Cluster:  ");
  Serial1.println(SD.blocksPerCluster());
  Serial1.print("Blocks size:  ");
  Serial1.println(SD.blockSize());

  Serial.print("Cluster size:          ");
  Serial.println(SD.clusterSize());
  Serial.print("Blocks x Cluster:  ");
  Serial.println(SD.blocksPerCluster());
  Serial.print("Blocks size:  ");
  Serial.println(SD.blockSize());
  
  Serial1.print("Total Blocks:      ");
  Serial1.println(SD.totalBlocks());
  Serial1.println();
    
  Serial.print("Total Blocks:      ");
  Serial.println(SD.totalBlocks());
  Serial.println();

  Serial1.print("Total Cluster:      ");
  Serial1.println(SD.totalClusters());
  Serial1.println();
  
  Serial.print("Total Cluster:      ");
  Serial.println(SD.totalClusters());
  Serial.println();

  // print the type and size of the first FAT-type volume
  uint32_t volumesize;
  Serial1.print("Volume type is:    FAT");
  Serial1.println(SD.fatType(), DEC);

  Serial.print("Volume type is:    FAT");
  Serial.println(SD.fatType(), DEC);
  
  volumesize = SD.totalClusters();
  volumesize *= SD.clusterSize();
  volumesize /= 1000;
  Serial1.print("Volume size (Kb):  ");
  Serial1.println(volumesize);
  Serial1.print("Volume size (Mb):  ");

  Serial.print("Volume size (Kb):  ");
  Serial.println(volumesize);
  Serial.print("Volume size (Mb):  ");
  volumesize /= 1024;
  Serial1.println(volumesize);
  Serial1.print("Volume size (Gb):  ");
  Serial1.println((float)volumesize / 1024.0);

  Serial.println(volumesize);
  Serial.print("Volume size (Gb):  ");
  Serial.println((float)volumesize / 1024.0);
  
  Serial1.print("Card size:  ");
  Serial1.println((float)SD.size() / 1000);

  Serial.print("Card size:  ");
  Serial.println((float)SD.size() / 1000);
  FSInfo fs_info;
  SDFS.info(fs_info);

  Serial1.print("Total bytes: ");
  Serial1.println(fs_info.totalBytes);

  Serial1.print("Used bytes: ");
  Serial1.println(fs_info.usedBytes);

  Serial.print("Total bytes: ");
  Serial.println(fs_info.totalBytes);

  Serial.print("Used bytes: ");
  Serial.println(fs_info.usedBytes);
  root = SD.open("/");
  printDirectory(root, 0);
  
}
void printDirectory(File dir, int numTabs) {
  while (true) {

    File entry = dir.openNextFile();
    if (!entry) {
      // no more files
      break;
    }
    for (uint8_t i = 0; i < numTabs; i++) {
      Serial1.print('\t');
      Serial.print('\t');
    }
    Serial1.print(entry.name());
    Serial.print(entry.name());
    if (entry.isDirectory()) {
      Serial1.println("/");
      Serial.println("/");
      printDirectory(entry, numTabs + 1);
    } else {
      // files have sizes, directories do not
      Serial1.print("\t\t");
      Serial1.print(entry.size(), DEC);
      Serial.print("\t\t");
      Serial.print(entry.size(), DEC);
      time_t cr = entry.getCreationTime();
      time_t lw = entry.getLastWrite();
      struct tm* tmstruct = localtime(&cr);
      Serial1.printf("\tCREATION: %d-%02d-%02d %02d:%02d:%02d", (tmstruct->tm_year) + 1900, (tmstruct->tm_mon) + 1, tmstruct->tm_mday, tmstruct->tm_hour, tmstruct->tm_min, tmstruct->tm_sec);
      Serial.printf("\tCREATION: %d-%02d-%02d %02d:%02d:%02d", (tmstruct->tm_year) + 1900, (tmstruct->tm_mon) + 1, tmstruct->tm_mday, tmstruct->tm_hour, tmstruct->tm_min, tmstruct->tm_sec);
      tmstruct = localtime(&lw);
      Serial.printf("\tLAST WRITE: %d-%02d-%02d %02d:%02d:%02d\n", (tmstruct->tm_year) + 1900, (tmstruct->tm_mon) + 1, tmstruct->tm_mday, tmstruct->tm_hour, tmstruct->tm_min, tmstruct->tm_sec);
      Serial1.printf("\tLAST WRITE: %d-%02d-%02d %02d:%02d:%02d\n", (tmstruct->tm_year) + 1900, (tmstruct->tm_mon) + 1, tmstruct->tm_mday, tmstruct->tm_hour, tmstruct->tm_min, tmstruct->tm_sec);
    }
    entry.close();
  }

}

//SD卡初始化
// int SD_init()
// {

//   if (!SD.begin(SD_CS))
//   {
//     Serial.println("Card Mount Failed");
//     return 1;
//   }
//   uint8_t cardType = SD.cardType();

//   if (cardType == CARD_NONE)
//   {
//     Serial.println("No TF card attached");
//     return 1;
//   }

//   uint64_t cardSize = SD.cardSize() / (1024 * 1024);
//   Serial.printf("TF Card Size: %lluMB\n", cardSize);
//   listDir(SD, "/", 2);

//   //  listDir(SD, "/", 0);
//   //  createDir(SD, "/mydir");
//   //  listDir(SD, "/", 0);
//   //  removeDir(SD, "/mydir");
//   //  listDir(SD, "/", 2);
//   //  writeFile(SD, "/hello.txt", "Hello ");
//   //  appendFile(SD, "/hello.txt", "World!\n");
//   //  readFile(SD, "/hello.txt");
//   //  Serial.printf("Total space: %lluMB\n", SD.totalBytes() / (1024 * 1024));
//   //  Serial.printf("Used space: %lluMB\n", SD.usedBytes() / (1024 * 1024));
//   //  Serial.println("SD init over.");

//   return 0;
// }

// //遍历SD卡目录
// void listDir(fs::FS & fs, const char *dirname, uint8_t levels)
// {
//   //  Serial.printf("Listing directory: %s\n", dirname);

//   File root = fs.open(dirname);
//   if (!root)
//   {
//     //Serial.println("Failed to open directory");
//     return;
//   }
//   if (!root.isDirectory())
//   {
//     Serial.println("Not a directory");
//     return;
//   }

//   File file = root.openNextFile();
// //  i = 0;
//   while (file)
//   {
//     if (file.isDirectory())
//     {
//       //      Serial.print("  DIR : ");
//       //      Serial.println(file.name());
//       if (levels)
//       {
//         listDir(fs, file.name(), levels - 1);
//       }
//     }
//     else
//     {
//       Serial.print("FILE: ");
//       Serial.print(file.name());
// //      lcd.setCursor(0, 2 * i);
// //      lcd.printf("FILE:%s", file.name());

//       Serial.print("SIZE: ");
//       Serial.println(file.size());
// //      lcd.setCursor(180, 2 * i);
// //      lcd.printf("SIZE:%d", file.size());
// //      i += 16;
//     }

//     file = root.openNextFile();
//   }
// }
