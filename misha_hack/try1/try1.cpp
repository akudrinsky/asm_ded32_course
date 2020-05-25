#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

const char* colors[] = {
    "\033[0;31m",           // red
    "\033[0;33m",           // yellow
    "\033[0;34m",           // blue
    "\033[0;32m",           // green
    "\033[0;36m",           // cian
    "\033[0;35m",           // purple
};

const char NO_COLOR[]   = "\033[0m";

const char* rand_color () {
    return colors[abs (rand ()) % (sizeof (colors) / sizeof (const char*))];
}

const char* thoughts [] = {
    "узнал про Ролля - бахнул алкоголя\nузнал про Планка - ударил в рожу панка\nузнал про Коши - взорвал анаши",
    "Препод в зуме о матAне -\nЯ с девчонкой на диване...",
    "I'm doing math\nAfter Turkish bath",
    "Как ботал - так и заработал",
    "Мы идем к успеху\nНа лабах по элтеху",
    "Читал по теорверу книжки\nЗабрал в покере все фишки",
    "Пока ты клеишь рисунки к лабам,\nмы с пацанами пишем бабам",
    "Сдал Дединскому я прогу, чтоб позволить дом бульдогу",
    "Ты доказываешь в суде, а мы - на пересде",
    "я помог ей с физосом\nона оставила меня с засосом",
    "Шуба, даймонд, цацки, yeezy\nЯ обычный астрофизик",
    "Я учусь, чтоб приносить в семью бабосы\nНо приношу пока что лишь удосы",
};

const char* rand_thought () {
    return thoughts[abs (rand ()) % (sizeof (thoughts) / sizeof (const char*))];
}

int main (int argc, const char* argv[]) {
	//system ("cp ../src/macos_security_check leaked_misha");
    srand (time (nullptr));
    
    if (argc == 1) {
        printf ("No file name entered.\n");
        return 0;
    }
    
	FILE* leaked = fopen (argv[1], "rb+");
    if (leaked == nullptr) {
        printf ("File was not found.\n");
        return 0;
    }
    
    printf ("%sNow there is a super-secure program. We will hack it with patch.\n", rand_color ());
    sleep (2);
    printf ("%sSee some clever russian thoughts, while I will do all the dirty work.\n", rand_color ());
    sleep (3);
    
    for (int i = 0; i < 12; ++i) {
        printf ("%s%s\n", rand_color (), rand_thought ());
        sleep (1);
    }
	
	fseek (leaked, 0xcca, SEEK_SET);
	
	fputc (0x90, leaked);
	fputc (0x90, leaked);

	fclose (leaked);
    
    printf ("%sReady. See file %s in the same directory, where it was.\n", NO_COLOR, argv[1]);
}	
