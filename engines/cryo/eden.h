/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#ifndef CRYO_EDEN_H
#define CRYO_EDEN_H

#include "cryo/defs.h"

enum Direction {
	kCryoNorth = 0,
	kCryoEast = 1,
	kCryoSouth = 2,
	kCryoWest = 3
};

#define EDEN_DEBUG

namespace Cryo {

class EdenGame {
public:
	void run();

private:
	void removeConsole();
	void scroll();
	void resetScroll();
	void scrollFrescoes();
	void displayFrescoes();
	void gametofresques();
	void doFrescoes();
	void endFrescoes();
	void scrollMirror();
	void scrollpano();
	void affsuiveur(suiveur_t *suiveur, int16 x, int16 y);
	void persoinmiroir();
	void gametomiroir(byte arg1);
	void flipMode();
	void quitMirror();
	void clictimbre();
	void clicplanval();
	void gotolieu(goto_t *go);
	void deplaval(uint16 roomNum);
	void move(Direction dir);
	void move2(Direction dir);
	void dinosoufle();
	void plaquemonk();
	void fresquesgraa();
	void fresqueslasc();
	void pushpierre();
	void tetemomie();
	void tetesquel();
	void squelmoorkong();
	void choisir();
	void dinaparle();
	void roiparle();
	void roiparle1();
	void roiparle2();
	void roiparle3();
	void getcouteau();
	void getprisme();
	void getchampb();
	void getchampm();
	void getor();
	void getnido();
	void getnidv();
	void getcorne();
	void getsoleil();
	void getoeuf();
	void getplaque();
	void voirlac();
	void gotohall();
	void demitourlabi();
	void gotonido();
	void gotoval();
	void visiter();
	void final();
	void moveNorth();
	void moveEast();
	void moveSouth();
	void moveWest();
	void afficher();
	void afficher128();
	void saveFriezes();
	void saveTopFrieze(int16 x);
	void saveBottomFrieze();
	void restoreFriezes();
	void restoreTopFrieze();
	void restoreBottomFrieze();
	void use_main_bank();
	void use_bank(int16 bank);
	void sundcurs(int16 x, int16 y);
	void rundcurs();
	void noclipax(int16 index, int16 x, int16 y);
	void noclipax_avecnoir(int16 index, int16 x, int16 y);
	void getglow(int16 x, int16 y, int16 w, int16 h);
	void unglow();
	void glow(int16 index);
	void readpalette(byte *ptr);
	void spritesurbulle(int16 index, int16 x, int16 y);
	void bars_out();
	void bars_in();
	void sauvefondbouche();
	void restaurefondbouche();
	void blackbars();
	void drawTopScreen();
	void affplanval();
	void affrepere(int16 index, int16 location);
	void affrepereadam(int16 location);
	void rest_repadam();
	void save_repadam(int16 x, int16 y);
	char istrice(int16 roomNum);
	char istyran(int16 roomNum);
	void istyranval(area_t *area);
	char getdirection(perso_t *perso);
	char caselibre(char loc, perso_t *perso);
	void melange1(char elem[4]);
	void melange2(char elem[4]);
	void melangedir();
	char naitredino(char persoType);
	void newcita(char arg1, int16 arg2, room_t *room);
	void citaevol(int16 level);
	void citacapoute(int16 roomNum);
	void buildcita();
	void citatombe(char level);
	void constcita();
	void depladino(perso_t *perso);
	void deplaalldino();
	void newvallee();
	char whereiscita();
	char iscita(int16 loc);
	void lieuvava(area_t *area);
	void vivredino();
	void vivreval(int16 areaNum);
	void chaquejour();
	void temps_passe(int16 t);
	void heurepasse();
	void anim_perso();
	void getanimrnd();
	void addanim();
	void virespritebouche();
	void anim_perfin();
	void perso_spr(byte *spr);
	void af_image();
	void af_perso1();
	void af_perso();
	void ef_perso();
	void load_perso(perso_t *perso);
	void load_perso_cour();
	void fin_perso();
	void no_perso();
	void close_perso();
	void af_fondsuiveur();
	void af_fondperso1();
	void af_fondperso();
	void setpersoicon();
	void show_perso();
	void showpersopanel();
	void getdatasync();
	int16 ReadNombreFrames();
	void waitendspeak();
	void my_bulle();
	void my_pr_bulle();
	void charsurbulle(byte c, byte color, int16 width);
	void af_subtitle();
	void sauvefondbulle(int16 y);
	void restaurefondbulle();
	void af_subtitlehnm();
	void patchphrase();
	void vavapers();
	void citadelle();
	void choixzone();
	void showevents1();
	void showEvents();
	void parle_mfin();
	void parlemoi_normal();
	void parle_moi();
	void init_perso_ptr(perso_t *perso);
	void perso1(perso_t *perso);
	void perso_normal(perso_t *perso);
	void persoparle(int16 pers);
	void roi();
	void dina();
	void thoo();
	void monk();
	void bourreau();
	void messager();
	void mango();
	void eve();
	void azia();
	void mammi();
	void gardes();
	void bambou();
	void kabuka();
	void fisher();
	void dino();
	void tyran();
	void morkus();
	void comment();
	void adam();
	void init_oui();
	void init_non();
	void oui();
	void non();
	char verif_oui();
	void SpcChampi(perso_t *perso);
	void SpcNidv(perso_t *perso);
	void SpcNido(perso_t *perso);
	void SpcPomme(perso_t *perso);
	void SpcOr(perso_t *perso);
	void SpcPrisme(perso_t *perso);
	void SpcTalisman(perso_t *perso);
	void SpcMasque(perso_t *perso);
	void SpcSac(perso_t *perso);
	void SpcTrompet(perso_t *perso);
	void SpcArmes(perso_t *perso);
	void SpcInstru(perso_t *perso);
	void SpcOeuf(perso_t *perso);
	void TyranMeurt(perso_t *perso);
	void SpecialObjets(perso_t *perso, char objid);
	void dialautoon();
	void dialautooff();
	void follow();
	void dialonfollow();
	void abortdial();
	void narrateur();
	void vrf_phrases_file();
	byte *gettxtad(int16 id);
	void gotocarte();
	void record();
	char dial_scan(dial_t *dial);
	char dialoscansvmas(dial_t *dial);
	char dialo_even(perso_t *perso);
	void stay_here();
	void mort(int16 vid);
	void evenchrono();
	void chronoon(int16 t);
	void prechargephrases(int16 vid);
	void effet1();
	void effet2();
	void effet3();
	void effet4();
	void ClearScreen();
	void colimacon(int16 pattern[16]);
	void fadetoblack(int delay);
	void fadetoblack128(int delay);
	void fadefromblack128(int delay);
	void rectanglenoir32();
	void setRS1(int16 sx, int16 sy, int16 ex, int16 ey);
	void setRD1(int16 sx, int16 sy, int16 ex, int16 ey);
	void wait(int howlong);
	void effetpix();
	void verifh(void *ptr);
	void openbigfile();
	void closebigfile();
	void loadfile(uint16 num, void *buffer);
	void shnmfl(uint16 num);
	int ssndfl(uint16 num);
	void ConvertIcons(icon_t *icon, int count);
	void ConvertLinks(room_t *room, int count);
	void ConvertMacToPC();
	void loadpermfiles();
	char ReadDataSync(uint16 num);
	void loadpartoffile(uint16 num, void *buffer, long pos, long len);
	void Expand_hsq(void *input, void *output);
	void ajouinfo(byte info);
	void unlockinfo();
	void nextinfo();
	void delinfo(byte info);
	void updateinfolist();
	void init_globals();
	void initrect();
	void closesalle();
	void afsalle1(room_t *room);
	void afsalle();
	void aflieu();
	void loadsal(int16 num);
	void specialoutside();
	void specialout();
	void specialin();
	void animpiece();
	void getdino(room_t *room);
	room_t *getsalle(int16 loc);
	void initlieu(int16 roomNum);
	void maj2();
	void majsalle1(int16 roomNum);
	void maj_salle(uint16 roomNum);
	void initbuf();
	void freebuf();
	void openwindow();
	void EmergencyExit();
	void edmain();
	void intro();
	char testcdromspeed();
	void entergame();
	void signon(char *s);
	void testPommeQ();
	void FRDevents();
	icon_t *scan_icon_list(int16 x, int16 y, int16 index);
	void update_cursor();
	void mouse();
	void showMovie(char arg1);
	void playHNM(int16 num);
	void displayHNMSubtitles();
	void musique();
	void startmusique(byte num);
	void musicspy();
	int loadmusicfile(int16 num);
	void persovox();
	void endpersovox();
	void fademusicup();
	void fademusica0(int16 delay);
	object_t *getobjaddr(int16 id);
	void countobjects();
	void showObjects();
	void winobject(int16 id);
	void loseObject(int16 id);
	void lostobject();
	char objecthere(int16 id);
	void objectmain(int16 id);
	void getobject(int16 id);
	void putobject();
	void newobject(int16 id, int16 arg2);
	void giveobjectal(int16 id);
	void giveobject();
	void takeobject();
	void newchampi();
	void newnidv();
	void newnido();
	void newor();
	void gotopanel();
	void noclicpanel();
	void generique();
	void cancel2();
	void testvoice();
	void load();
	void initafterload();
	void save();
	void desktopcolors();
	void panelrestart();
	void reallyquit();
	void confirmer(char mode, char yesId);
	void confirmyes();
	void confirmno();
	void restart();
	void EdenQuit();
	void choixsubtitle();
	void reglervol();
	void changervol();
	void newvol(byte *volptr, int16 delta);
	void playtape();
	void rewindtape();
	void depcurstape();
	void affcurstape();
	void forwardtape();
	void stoptape();
	void cliccurstape();
	void paneltobuf();
	void cursbuftopanel();
	void langbuftopanel();
	void affpanel();
	void afflangue();
	void affcursvol(int16 x, int16 vol1, int16 vol2);
	void affcurseurs();
	void curseurselect(int itemId);
	void afftoppano();
	void affresult();
	void limitezonecurs(int16 xmin, int16 xmax, int16 ymin, int16 ymax);
	void PommeQ();
	void habitants(perso_t *perso);
	void suiveurs(perso_t *perso);
	void evenements(perso_t *perso);
	void followme(perso_t *perso);
	void rangermammi(perso_t *perso, room_t *room);
	void perso_ici(int16 action);
	void setpersohere();
	void faire_suivre(int16 roomNum);
	void suis_moi5();
	void suis_moi(int16 index);
	void reste_ici5();
	void reste_ici(int16 index);
	void eloipart();
	char eloirevientq();
	void eloirevient();
	void incphase1();
	void incphase();
	void phase113();
	void phase130();
	void phase161();
	void phase226();
	void phase257();
	void phase353();
	void phase369();
	void phase371();
	void phase385();
	void phase418();
	void phase433();
	void phase434();
	void phase513();
	void phase514();
	void phase529();
	void phase545();
	void phase561();
	void bigphase1();
	void bigphase();
	void phase16();
	void phase32();
	void phase48();
	void phase64();
	void phase80();
	void phase96();
	void phase112();
	void phase128();
	void phase144();
	void phase160();
	void phase176();
	void phase192();
	void phase208();
	void phase224();
	void phase240();
	void phase256();
	void phase272();
	void phase288();
	void phase304();
	void phase320();
	void phase336();
	void phase352();
	void phase368();
	void phase384();
	void phase400();
	void phase416();
	void phase432();
	void phase448();
	void phase464();
	void phase480();
	void phase496();
	void phase512();
	void phase528();
	void phase544();
	void phase560();
	void savegame(char *name);
	void loadrestart();
	void loadgame(char *name);
	void vavaoffsetout();
	void vavaoffsetin();
	void lieuoffsetout();
	void lieuoffsetin();
	void bandeoffsetout();
	void bandeoffsetin();
	char testcondition(int16 index);
	uint16 opera_add(uint16 v1, uint16 v2);
	uint16 opera_sub(uint16 v1, uint16 v2);
	uint16 opera_and(uint16 v1, uint16 v2);
	uint16 opera_or(uint16 v1, uint16 v2);
	uint16 opera_egal(uint16 v1, uint16 v2);
	uint16 opera_petit(uint16 v1, uint16 v2);
	uint16 opera_grand(uint16 v1, uint16 v2);
	uint16 opera_diff(uint16 v1, uint16 v2);
	uint16 opera_petega(uint16 v1, uint16 v2);
	uint16 opera_graega(uint16 v1, uint16 v2);
	uint16 opera_faux(uint16 v1, uint16 v2);
	uint16 operation(byte op, uint16 v1, uint16 v2);
	uint16 cher_valeur();
	void monbreak();
	void ret();
	void make_tabcos();
	void make_matrice_fix();
	void projection_fix(cube_t *cube, int n);
	void init_cube();
	void moteur();
	void affiche_objet(cube_t *cube);
	void NEWcharge_map(int file_id, byte *buffer);
	void NEWcharge_objet_mob(cube_t *cube, int file_id, byte *texptr);
	static int next_val(char **ptr, char *error);
	void selectmap(int16 num);
	void Eden_dep_and_rot();
	void restoreZDEP();
	void affiche_polygone_mapping(cube_t *cube, cubeface_t *face);
	void trace_ligne_mapping(int16 r3, int16 r4, int16 r5, int16 r6, int16 r7, int16 r8, int16 r9, int16 r10, int16 *lines);
	void affiche_ligne_mapping(int16 r3, int16 r4, byte *target, byte *texture);
	int16 OpenDialog(void *arg1, void *arg2);
	void LostEdenMac_InitPrefs();

private:
	int16           old_scroll_pos, scroll_pos;
	int16           word_2F514;
	bool   frescoTalk;
	byte   keep01, keep02, keep10, keep11, keep12, keep13, keep21, keep22;
	byte   curs_keepbuf[2500];
	int16           curs_keepy, curs_keepx;
	bool            torchCursor;
	int16           cur_bank_num;
	int16           glow_h;
	int16           glow_w;
	int16           glow_y;
	int16           glow_x;
	byte   needPaletteUpdate;
	byte   curs_saved;
	byte   showBlackBars;
	byte   fond_saved;
	byte   *bank_data_ptr;
	color3_t        pal_entry;
	color_t         global_palette[256];    //TODO palette_t
	perso_t         *tyranPtr;
	int             last_anim_frame_num;
	int             cur_anim_frame_num;
	int             last_anim_ticks;
	prect_t         *cur_perso_rect;
	int16           num_anim_frames;
	int16           max_perso_desc;
	int16           num_img_desc;
	byte   restartAnimation;
	byte   animationActive;
	byte   animationDelay;
	byte   animationIndex;
	byte   lastAnimationIndex;

	byte   *dword_30724;
	byte   *dword_30728;   //TODO: rename - something amim-related
	byte   *dword_3072C;   //TODO ditto
	byte   *animationTable;
	byte   imagedesc[512];
	byte   *perso_img_bank_data_ptr;
	byte   savedUnderSubtitles;
	int16           num_text_lines;
	byte   phraseBuffer[400];
	byte   *text_ptr;
	byte   phraseIconsBuffer[10];
	byte   phraseCoordsBuffer[22];
	byte   *textoutptr;
	byte   *textout;
	object_t        *currentSpecialObject;
	int16           word_30AFC;
	byte   byte_30AFE;

	byte   byte_30B00;
	int             dword_30B04;

	char            lastPhrasesFile;
	byte dialogSkipFlags;

	color3_t        newColor;
	color_t         oldPalette[256];    // TODO palette_t ?
	color_t         newPalette[256];
	rect_t          rect_dst, rect_src;
	void            *voiceSamplesBuffer;    //TODO: sound sample buffer
	file_t          h_bigfile;
	byte   info_list[16];
	byte   needToFade;
	byte   lastMusicNum;
	byte   *main_bank_buf;
	byte   *music_buf;
	byte   *gameLipsync;
	byte   *gamePhrases;
	byte   *gameDialogs;   //TODO: rename to dialogs?
	byte   *gameConditions;
	void            *sal_buf;   //TODO: fixme
	byte   *bank_data_buf;
	icon_t          *gameIcons;
	room_t          *gameRooms;
	pak_t           *bigfile_header;
	byte   *glow_buffer;
	byte   *p_mainview_buf;
	byte   *p_view2_buf;
	byte   *gameFont;  //TODO: rename to font?
	byte   *p_subtitlesview_buf;
	byte   *p_underSubtitlesView_buf;
	global_t        *p_global;
	uint16  mouse_y_center, mouse_x_center;
	int             quit_flag3;     //TODO: some obsolete error flag?
	uint16  machine_speed;
	byte   quit_flag;

	byte   gameStarted;

	byte   quit_flag2;
	byte   soundAllocated;
	soundchannel_t  *music_channel;
	soundchannel_t  *hnmsound_ch;
	sound_t         *voiceSound;

	view_t          *p_view2;
	view_t          *p_underSubtitlesView;
	view_t          *p_subtitlesview;
	view_t          *p_underBarsView;
	view_t          *p_mainview;
	view_t          *p_hnmview;
	hnm_t           *p_hnmcontext;
	filespec_t      bigfilespec;
	rect_t          underSubtitlesBackupRect, underSubtitlesScreenRect, underBottomBarBackupRect, underBottomBarScreenRect,
	                underTopBarBackupRect, underTopBarScreenRect, rect_31C7A;
	int             demoCurrentTicks;
	int             demoStartTicks;
	int             currentTime;
	int16           mouse_y;
	int16           mouse_x;
	int16           doubled;
	int16           curs_x_pan;
	int16           inventoryScrollDelay;
	int16           curs_y, curs_x;
	int16           current_cursor;
	icon_t          *current_spot;
	icon_t          *current_spot2;
	byte   pomme_q;
	byte   keybd_held;
	byte   mouse_held;
	byte   normalCursor;
	byte   *p_hnmview_buf;
	byte   showVideoSubtitle;
	byte   videoCanceled;  //TODO: hnm_canceled
	bool   specialTextMode;
	int             hnm_position;
	int             voiceSamplesSize;   //TODO: perso vox sample data len
	int16           mus_vol_right;
	int16           mus_vol_left;


	byte   animateTalking;
	byte   personTalking;
	byte   mus_fade_flags;

	char            musicSequencePos;
	byte   musicPlaying;

	byte   *mus_samples_ptr;
	byte   *mus_patterns_ptr;  //TODO: sndblock_t ?
	byte   *mus_sequence_ptr;
	soundgroup_t    *mus_queue_grp;
	int16           *pCurrentObjectLocation;
	byte   own_objects[128];
	bool   byte_31D64;

	byte   no_palette;
	byte   gameLoaded;
#define MAX_TAPES 16
	tape_t          tapes[MAX_TAPES];
	byte   confirmMode;
	byte   *cur_slider_value_ptr;
	byte   lastMenuItemIdLo;
	int16           lastTapeRoomNum;
	int16           cur_slider_x;
	int16           cur_slider_y;
	int16           destinationRoom;
	int16           word_31E7A;

	int16           word_378CC; //TODO: set by CLComputer_Init to 0
	int16           word_378CE;

};

}

#endif
