#include <Window.h>
#include <TextView.h>
#include <StringView.h>
#include <String.h>
#include <Box.h>

#include "memmap.h"
#include "S9xRomInfo.h"

#define NOTKNOWN "Unknown Company "

S9xRomWindow::S9xRomWindow(BRect rect)
	: BWindow(rect, "Rom Information", B_FLOATING_WINDOW, B_NOT_ZOOMABLE | B_NOT_RESIZABLE)
{
	// this code was pasted from Win32 Snes9x rominfo code		
	BString romInfoStr("");
	BString nameStr(Memory.ROMFilename);
	int place = nameStr.FindLast('/');
	
	BString nameStr2(nameStr.String(), place+1);
	nameStr = nameStr.Remove(0, place+1);
	
	romInfoStr << "File Name: " << nameStr.String() << "\n";
	romInfoStr << "Location: " << nameStr2.String() << "\n";

	BString str(((Memory.ROMSpeed&0x10)!=0) ? "FastROM" : "SlowROM");
	romInfoStr << "Speed: " << Memory.ROMSpeed << "" << str.String() << "\n";
	
	str = BString(((Memory.ROMSpeed&0x0F)!=0) ? "HiROM" : "LoROM");
	romInfoStr << "ROM Map: " << str.String() << "\n";
	
	romInfoStr << "Type: " << Memory.ROMType << "\n";
	romInfoStr << "Kart contents: " << Memory.KartContents() << "\n";
	romInfoStr << "Header ROM Size: " << Memory.Size() << "\n";
	romInfoStr << "Calculated ROM Size: " << Memory.CalculatedSize/0x20000 << " Mbits" << "\n";
	romInfoStr << "SRAM size: " << Memory.StaticRAMSize() << "\n";
	romInfoStr << "Actual Checksum: " << Memory.CalculatedChecksum << "\n";
	romInfoStr << "Header Checksum: " << Memory.ROMChecksum << "\n";
	romInfoStr << "Header Checksum Compliment: " << Memory.ROMComplementChecksum << "\n";

	str = BString((Memory.ROMRegion > 12 || Memory.ROMRegion < 2) ? "NTSC 60HZ" : "PAL 50HZ");
	romInfoStr << "Output: " << str.String() << "\n";
	
	romInfoStr << "CRC32; " << Memory.ROMCRC32 << "\n";
	
	char romtext[4096] = "\0";
	strcpy(romtext, "Licensee: ");

	int tmp=-1;
	if(Memory.CompanyId[0]=='0')
		tmp=0;
	if(Memory.CompanyId[0]=='1')
		tmp=16;
	if(Memory.CompanyId[0]=='2')
		tmp=32;
	if(Memory.CompanyId[0]=='3')
		tmp=48;
	if(Memory.CompanyId[0]=='4')
		tmp=64;
	if(Memory.CompanyId[0]=='5')
		tmp=80;
	if(Memory.CompanyId[0]=='6')
		tmp=96;
	if(Memory.CompanyId[0]=='7')
		tmp=112;
	if(Memory.CompanyId[0]=='8')
		tmp=128;
	if(Memory.CompanyId[0]=='9')
		tmp=144;
	if(Memory.CompanyId[0]=='A')
		tmp=160;
	if(Memory.CompanyId[0]=='B')
		tmp=176;
	if(Memory.CompanyId[0]=='C')
		tmp=192;
	if(Memory.CompanyId[0]=='D')
		tmp=208;
	if(Memory.CompanyId[0]=='E')
		tmp=224;
	if(Memory.CompanyId[0]=='F')
		tmp=240;
	if(tmp!=-1)
	{
		if(Memory.CompanyId[1]=='0')
			tmp+=0;
		else if(Memory.CompanyId[1]=='1')
			tmp+=1;
		else if(Memory.CompanyId[1]=='2')
			tmp+=2;
		else if(Memory.CompanyId[1]=='3')
			tmp+=3;
		else if(Memory.CompanyId[1]=='4')
			tmp+=4;
		else if(Memory.CompanyId[1]=='5')
			tmp+=5;
		else if(Memory.CompanyId[1]=='6')
			tmp+=6;
		else if(Memory.CompanyId[1]=='7')
			tmp+=7;
		else if(Memory.CompanyId[1]=='8')
			tmp+=8;
		else if(Memory.CompanyId[1]=='9')
			tmp+=9;
		else if(Memory.CompanyId[1]=='A')
			tmp+=10;
		else if(Memory.CompanyId[1]=='B')
			tmp+=11;
		else if(Memory.CompanyId[1]=='C')
			tmp+=12;
		else if(Memory.CompanyId[1]=='D')
			tmp+=13;
		else if(Memory.CompanyId[1]=='E')
			tmp+=14;
		else if(Memory.CompanyId[1]=='F')
			tmp+=15;
		else tmp=0;
	}
		else tmp=0;
	if(tmp==0)
		tmp=((Memory.ROMSpeed&0x0F)!=0)?Memory.ROM[0x0FFDA]:Memory.ROM[0x7FDA];
		switch(tmp)
		{
			case 0:strcat(romtext, "INVALID company");break;
			case 1:strcat(romtext, "Nintendo");break;
			case 2:strcat(romtext, NOTKNOWN "2");break;
			case 3:strcat(romtext, "Imagineer-Zoom");break;
			case 4:strcat(romtext, NOTKNOWN "4");break;
			case 5:strcat(romtext, "Zamuse");break;
			case 6:strcat(romtext, "Falcom");break;
			case 7:strcat(romtext, NOTKNOWN "7");break;
			case 8:strcat(romtext, "Capcom");break;
			case 9:strcat(romtext, "HOT-B");break;
			case 10:strcat(romtext, "Jaleco");break;
			case 11:strcat(romtext, "Coconuts");break;
			case 12:strcat(romtext, "Rage Software");break;
			case 13:strcat(romtext, NOTKNOWN "13");break;
			case 14:strcat(romtext, "Technos");break;
			case 15:strcat(romtext, "Mebio Software");break;
			case 16:strcat(romtext, NOTKNOWN "16");break;
			case 17:strcat(romtext, NOTKNOWN "17");break;
			case 18:strcat(romtext, "Gremlin Graphics");break;
			case 19:strcat(romtext, "Electronic Arts");break;
			case 20:strcat(romtext, NOTKNOWN "20");break;
			case 21:strcat(romtext, "COBRA Team");break;
			case 22:strcat(romtext, "Human/Field");break;
			case 23:strcat(romtext, "KOEI");break;
			case 24:strcat(romtext, "Hudson Soft");break;
			case 25:strcat(romtext, NOTKNOWN "25");break;
			case 26:strcat(romtext, "Yanoman");break;
			case 27:strcat(romtext, NOTKNOWN "27");break;
			case 28:strcat(romtext, "Tecmo");break;
			case 29:strcat(romtext, NOTKNOWN "29");break;
			case 30:strcat(romtext, "Open System");break;
			case 31:strcat(romtext, "Virgin Games");break;
			case 32:strcat(romtext, "KSS");break;
			case 33:strcat(romtext, "Sunsoft");break;
			case 34:strcat(romtext, "POW");break;
			case 35:strcat(romtext, "Micro World");break;
			case 36:strcat(romtext, NOTKNOWN "36");break;
			case 37:strcat(romtext, NOTKNOWN "37");break;
			case 38:strcat(romtext, "Enix");break;
			case 39:strcat(romtext, NOTKNOWN "39");break;
			case 40:strcat(romtext, "Kemco");break;
			case 41:strcat(romtext, "Seta Co.,Ltd.");break;
			case 42:strcat(romtext, NOTKNOWN "42");break;
			case 43:strcat(romtext, "Irem Japan");break;//Irem? Gun Force J
			case 44:strcat(romtext, NOTKNOWN "44");break;
			case 45:strcat(romtext, "Visit Co.,Ltd.");break;
			case 46:strcat(romtext, NOTKNOWN "46");break;
			case 47:strcat(romtext, NOTKNOWN "47");break;
			case 48:strcat(romtext, NOTKNOWN "48");break;
			case 49:strcat(romtext, "Carrozzeria");break;
			case 50:strcat(romtext, "Dynamic");break;
			case 51:strcat(romtext, "Nintendo");break;
			case 52:strcat(romtext, "Magifact");break;
			case 53:strcat(romtext, "Hect");break;
			case 54:strcat(romtext, NOTKNOWN "54");break;
			case 55:strcat(romtext, NOTKNOWN "55");break;
			case 56:strcat(romtext, "Capcom Europe");break;//Capcom? BOF2(E) MM7 (E)
			case 57:strcat(romtext, "Accolade Europe");break;//Accolade?Bubsy 2 (E)
			case 58:strcat(romtext, NOTKNOWN "58");break;
			case 59:strcat(romtext, NOTKNOWN "59");break;
			case 60:strcat(romtext, "Empire Software");break;
			case 61:strcat(romtext, "Loriciel");break;
			case 62:strcat(romtext, NOTKNOWN "62");break;
			case 63:strcat(romtext, NOTKNOWN "63");break;
			case 64:strcat(romtext, "Seika Corp.");break;
			case 65:strcat(romtext, "UBI Soft");break;
			case 66:strcat(romtext, NOTKNOWN "66");break;
			case 67:strcat(romtext, NOTKNOWN "67");break;
			case 68:strcat(romtext, "LifeFitness Exertainment");break;//?? Exertainment Mountain Bike Rally (U).zip
			case 69:strcat(romtext, NOTKNOWN "69");break;
			case 70:strcat(romtext, "System 3");break;
			case 71:strcat(romtext, "Spectrum Holobyte");break;
			case 72:strcat(romtext, NOTKNOWN "72");break;
			case 73:strcat(romtext, "Irem");break;
			case 74:strcat(romtext, NOTKNOWN "74");break;
			case 75:strcat(romtext, "Raya Systems/Sculptured Software");break;
			case 76:strcat(romtext, "Renovation Products");break;
			case 77:strcat(romtext, "Malibu Games/Black Pearl");break;
			case 78:strcat(romtext, NOTKNOWN "78");break;
			case 79:strcat(romtext, "U.S. Gold");break;
			case 80:strcat(romtext, "Absolute Entertainment");break;
			case 81:strcat(romtext, "Acclaim");break;
			case 82:strcat(romtext, "Activision");break;
			case 83:strcat(romtext, "American Sammy");break;
			case 84:strcat(romtext, "GameTek");break;
			case 85:strcat(romtext, "Hi Tech Expressions");break;
			case 86:strcat(romtext, "LJN Toys");break;
			case 87:strcat(romtext, NOTKNOWN "87");break;
			case 88:strcat(romtext, NOTKNOWN "88");break;
			case 89:strcat(romtext, NOTKNOWN "89");break;
			case 90:strcat(romtext, "Mindscape");break;
			case 91:strcat(romtext, NOTKNOWN "91");break;
			case 92:strcat(romtext, NOTKNOWN "92");break;
			case 93:strcat(romtext, "Tradewest");break;
			case 94:strcat(romtext, NOTKNOWN "94");break;
			case 95:strcat(romtext, "American Softworks Corp.");break;
			case 96:strcat(romtext, "Titus");break;
			case 97:strcat(romtext, "Virgin Interactive Entertainment");break;
			case 98:strcat(romtext, "Maxis");break;
			case 99:strcat(romtext, NOTKNOWN "99");break;
			case 100:strcat(romtext, NOTKNOWN "100");break;
			case 101:strcat(romtext, NOTKNOWN "101");break;
			case 102:strcat(romtext, NOTKNOWN "102");break;
			case 103:strcat(romtext, "Ocean");break;
			case 104:strcat(romtext, NOTKNOWN "104");break;
			case 105:strcat(romtext, "Electronic Arts");break;
			case 106:strcat(romtext, NOTKNOWN "106");break;
			case 107:strcat(romtext, "Laser Beam");break;
			case 108:strcat(romtext, NOTKNOWN "108");break;
			case 109:strcat(romtext, NOTKNOWN "109");break;
			case 110:strcat(romtext, "Elite");break;
			case 111:strcat(romtext, "Electro Brain");break;
			case 112:strcat(romtext, "Infogrames");break;
			case 113:strcat(romtext, "Interplay");break;
			case 114:strcat(romtext, "LucasArts");break;
			case 115:strcat(romtext, "Parker Brothers");break;
			case 116:strcat(romtext, NOTKNOWN "116");break;
			case 117:strcat(romtext, "STORM");break;
			case 118:strcat(romtext, NOTKNOWN "118");break;
			case 119:strcat(romtext, NOTKNOWN "119");break;
			case 120:strcat(romtext, "THQ Software");break;
			case 121:strcat(romtext, "Accolade Inc.");break;
			case 122:strcat(romtext, "Triffix Entertainment");break;
			case 123:strcat(romtext, NOTKNOWN "123");break;
			case 124:strcat(romtext, "Microprose");break;
			case 125:strcat(romtext, NOTKNOWN "125");break;
			case 126:strcat(romtext, NOTKNOWN "126");break;
			case 127:strcat(romtext, "Kemco");break;
			case 128:strcat(romtext, "Misawa");break;
			case 129:strcat(romtext, "Teichio");break;
			case 130:strcat(romtext, "Namco Ltd.");break;
			case 131:strcat(romtext, "Lozc");break;
			case 132:strcat(romtext, "Koei");break;
			case 133:strcat(romtext, NOTKNOWN "133");break;
			case 134:strcat(romtext, "Tokuma Shoten Intermedia");break;
			case 135:strcat(romtext, NOTKNOWN "135");break;
			case 136:strcat(romtext, "DATAM-Polystar");break;
			case 137:strcat(romtext, NOTKNOWN "137");break;
			case 138:strcat(romtext, NOTKNOWN "138");break;
			case 139:strcat(romtext, "Bullet-Proof Software");break;
			case 140:strcat(romtext, "Vic Tokai");break;
			case 141:strcat(romtext, NOTKNOWN "141");break;
			case 142:strcat(romtext, "Character Soft");break;
			case 143:strcat(romtext, "I\'\'Max");break;
			case 144:strcat(romtext, "Takara");break;
			case 145:strcat(romtext, "CHUN Soft");break;
			case 146:strcat(romtext, "Video System Co., Ltd.");break;
			case 147:strcat(romtext, "BEC");break;
			case 148:strcat(romtext, NOTKNOWN "148");break;
			case 149:strcat(romtext, "Varie");break;
			case 150:strcat(romtext, NOTKNOWN "150");break;
			case 151:strcat(romtext, "Kaneco");break;
			case 152:strcat(romtext, NOTKNOWN "152");break;
			case 153:strcat(romtext, "Pack in Video");break;
			case 154:strcat(romtext, "Nichibutsu");break;
			case 155:strcat(romtext, "TECMO");break;
			case 156:strcat(romtext, "Imagineer Co.");break;
			case 157:strcat(romtext, NOTKNOWN "157");break;
			case 158:strcat(romtext, NOTKNOWN "158");break;
			case 159:strcat(romtext, NOTKNOWN "159");break;
			case 160:strcat(romtext, "Telenet");break;
			case 161:strcat(romtext, NOTKNOWN "161");break;
			case 162:strcat(romtext, NOTKNOWN "162");break;
			case 163:strcat(romtext, NOTKNOWN "163");break;
			case 164:strcat(romtext, "Konami");break;
			case 165:strcat(romtext, "K.Amusement Leasing Co.");break;
			case 166:strcat(romtext, NOTKNOWN "166");break;
			case 167:strcat(romtext, "Takara");break;
			case 168:strcat(romtext, NOTKNOWN "168");break;
			case 169:strcat(romtext, "Technos Jap.");break;
			case 170:strcat(romtext, "JVC");break;
			case 171:strcat(romtext, NOTKNOWN "171");break;
			case 172:strcat(romtext, "Toei Animation");break;
			case 173:strcat(romtext, "Toho");break;
			case 174:strcat(romtext, NOTKNOWN "174");break;
			case 175:strcat(romtext, "Namco Ltd.");break;
			case 176:strcat(romtext, NOTKNOWN "176");break;
			case 177:strcat(romtext, "ASCII Co. Activison");break;
			case 178:strcat(romtext, "Bandai");break;
			case 179:strcat(romtext, NOTKNOWN "179");break;
			case 180:strcat(romtext, "Enix America");break;
			case 181:strcat(romtext, NOTKNOWN "181");break;
			case 182:strcat(romtext, "Halken");break;
			case 183:strcat(romtext, NOTKNOWN "183");break;
			case 184:strcat(romtext, NOTKNOWN "184");break;
			case 185:strcat(romtext, NOTKNOWN "185");break;
			case 186:strcat(romtext, "Culture Brain");break;
			case 187:strcat(romtext, "Sunsoft");break;
			case 188:strcat(romtext, "Toshiba EMI");break;
			case 189:strcat(romtext, "Sony Imagesoft");break;
			case 190:strcat(romtext, NOTKNOWN "190");break;
			case 191:strcat(romtext, "Sammy");break;
			case 192:strcat(romtext, "Taito");break;
			case 193:strcat(romtext, NOTKNOWN "193");break;
			case 194:strcat(romtext, "Kemco");break;
			case 195:strcat(romtext, "Square");break;
			case 196:strcat(romtext, "Tokuma Soft");break;
			case 197:strcat(romtext, "Data East");break;
			case 198:strcat(romtext, "Tonkin House");break;
			case 199:strcat(romtext, NOTKNOWN "199");break;
			case 200:strcat(romtext, "KOEI");break;
			case 201:strcat(romtext, NOTKNOWN "201");break;
			case 202:strcat(romtext, "Konami USA");break;
			case 203:strcat(romtext, "NTVIC");break;
			case 204:strcat(romtext, NOTKNOWN "204");break;
			case 205:strcat(romtext, "Meldac");break;
			case 206:strcat(romtext, "Pony Canyon");break;
			case 207:strcat(romtext, "Sotsu Agency/Sunrise");break;
			case 208:strcat(romtext, "Disco/Taito");break;
			case 209:strcat(romtext, "Sofel");break;
			case 210:strcat(romtext, "Quest Corp.");break;
			case 211:strcat(romtext, "Sigma");break;
			case 212:strcat(romtext, NOTKNOWN "212");break;
			case 213:strcat(romtext, NOTKNOWN "213");break;
			case 214:strcat(romtext, "Naxat");break;
			case 215:strcat(romtext, NOTKNOWN "215");break;
			case 216:strcat(romtext, "Capcom Co., Ltd.");break;
			case 217:strcat(romtext, "Banpresto");break;
			case 218:strcat(romtext, "Tomy");break;
			case 219:strcat(romtext, "Acclaim");break;
			case 220:strcat(romtext, NOTKNOWN "220");break;
			case 221:strcat(romtext, "NCS");break;
			case 222:strcat(romtext, "Human Entertainment");break;
			case 223:strcat(romtext, "Altron");break;
			case 224:strcat(romtext, "Jaleco");break;
			case 225:strcat(romtext, NOTKNOWN "225");break;
			case 226:strcat(romtext, "Yutaka");break;
			case 227:strcat(romtext, NOTKNOWN "227");break;
			case 228:strcat(romtext, "T&ESoft");break;
			case 229:strcat(romtext, "EPOCH Co.,Ltd.");break;
			case 230:strcat(romtext, NOTKNOWN "230");break;
			case 231:strcat(romtext, "Athena");break;
			case 232:strcat(romtext, "Asmik");break;
			case 233:strcat(romtext, "Natsume");break;
			case 234:strcat(romtext, "King Records");break;
			case 235:strcat(romtext, "Atlus");break;
			case 236:strcat(romtext, "Sony Music Entertainment");break;
			case 237:strcat(romtext, NOTKNOWN "237");break;
			case 238:strcat(romtext, "IGS");break;
			case 239:strcat(romtext, NOTKNOWN "239");break;
			case 240:strcat(romtext, NOTKNOWN "240");break;
			case 241:strcat(romtext, "Motown Software");break;
			case 242:strcat(romtext, "Left Field Entertainment");break;
			case 243:strcat(romtext, "Beam Software");break;
			case 244:strcat(romtext, "Tec Magik");break;
			case 245:strcat(romtext, NOTKNOWN "245");break;
			case 246:strcat(romtext, NOTKNOWN "246");break;
			case 247:strcat(romtext, NOTKNOWN "247");break;
			case 248:strcat(romtext, NOTKNOWN "248");break;
			case 249:strcat(romtext, "Cybersoft");break;
			case 250:strcat(romtext, NOTKNOWN "250");break;
			case 251:strcat(romtext, NOTKNOWN "251");break;
			case 252:strcat(romtext, NOTKNOWN "252");break;
			case 253:strcat(romtext, NOTKNOWN "253");break;
			case 254:strcat(romtext, NOTKNOWN "254");break;
			case 255:strcat(romtext, NOTKNOWN "255");break;
			default:strcat(romtext, NOTKNOWN);break;
		}
	
	romInfoStr << romtext << "\n";

	BFont headerFont(be_bold_font);
	headerFont.SetSize(14.0f);
	font_height fh1;
	headerFont.GetHeight(&fh1);
	BRect textRect1 = BRect(5, 5, Bounds().Width() - 10, fh1.leading + fh1.ascent + 10);

	int space = 10;
	
	BStringView *strView = new BStringView(textRect1, "header", Memory.ROMName, B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP);
	strView->SetFont(&headerFont);
	strView->SetAlignment(B_ALIGN_CENTER);

	textRect1.top = fh1.ascent + fh1.descent + space*2;
	textRect1.bottom = Bounds().bottom - space;
	BRect textRect = textRect1;
	textRect.top = 0;
	
	ResizeTo(romInfoStr.Length(), textRect.Height());
	
	BView* view = new BView(Bounds(),"View1",B_FOLLOW_NONE,B_WILL_DRAW);
	view->SetViewColor(216, 216, 216);
	
	BRect boxRect = BRect(Bounds().left + space, Bounds().top + space, Bounds().right - space, Bounds().bottom - space);
	BBox* boxGeneral = new BBox(boxRect, "fBoxGeneral", B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE, B_FANCY_BORDER);

	BRect rect = BRect(boxRect.left, textRect1.top, boxRect.Width() - space, boxRect.bottom - space*2);
	BTextView *txtView = new BTextView(rect, "infoText", textRect, B_FOLLOW_LEFT | B_FOLLOW_TOP);
	txtView->MakeEditable(false);
	txtView->SetViewColor(216,216,216,0);
	
	AddChild(view);
	view->AddChild(boxGeneral);
	boxGeneral->AddChild(strView);
	boxGeneral->AddChild(txtView);
	txtView->SetText(romInfoStr.String());
	Show();
}

