#define _CRT_SECURE_NO_WARNINGS
#define ERROR_MESSAGE "ОШИБКА: Недостаточно памяти для работы программы"
#include <iostream>

typedef struct {
    char* fileName; // имя файла
    char* artistName; // псевдоним исполнителя
    char* trackName; // название трека
    int duration; // длительность трека в секундах
    int releaseYear; // год выпуска трека
} AudioFile;

AudioFile* audioFileCreate(char const* fileName, char const* artistName, char const* trackName, int duration, int releaseYear);
void audioFileOutput(AudioFile* obj);
void audioFileShortOutput(AudioFile* obj);
void audioFileChangeTrackName(AudioFile* obj, char const* newTrackName);
void audioFileFree(AudioFile* obj);

typedef struct {
    AudioFile* list;
    int listLength;
} AudioFileArray;

AudioFileArray* audioFileArrayCreate();
int audioFileArrayAddElement(AudioFileArray* arr, AudioFile* elem);
int audioFileArrayRemoveElement(AudioFileArray* arr, int index);
int audioFileArrayOutput(AudioFileArray* arr);
int audioFileArrayShortOutput(AudioFileArray* arr);
void audioFileArrayFree(AudioFileArray* obj);

typedef struct {
    char* playlistName;
    AudioFileArray* list;
} Playlist;

Playlist* playlistCreate(char const* playlistName, AudioFileArray* audioFileArrayObj);
void playlistOutput(Playlist* obj);
void playlistAddElement(Playlist* playlistObj, AudioFile* audioFileObj);
void playlistRemoveElement(Playlist* obj, int index);
void playlistFree(Playlist* obj);

typedef struct {
    Playlist* list;
    int listLength;
} PlaylistArray;

PlaylistArray* playlistArrayCreate();
int playlistArrayAddElement(PlaylistArray* arr, Playlist* elem);
int playlistArrayRemoveElement(PlaylistArray* arr, int index);
int playlistArrayOutput(PlaylistArray* arr);
void playlistArrayFree(PlaylistArray* obj);

typedef struct {
    AudioFileArray* list;
} AudioCollection;

AudioCollection* audioCollectionCreate(AudioFileArray* list);
void audioCollectionAddElement(AudioCollection* obj, AudioFile* elem);
void audioCollectionRemoveElement(AudioCollection* obj, int index);
AudioFileArray* audioCollectionSearchByArtistName(AudioCollection* obj, char const* artistName);
AudioFileArray* audioCollectionSearchByReleaseYear(AudioCollection* obj, int releaseYear);
void audioCollectionPrintStats(AudioCollection* obj);
void audioCollectionFree(AudioCollection* obj);

typedef struct {
    Playlist* currPlaylist;
    int trackIndexInPlaylist;
    int isPlaying;
} AudioPlayer;

AudioPlayer* audioPlayerCreate(Playlist* playlistObj, int trackIndexInPlaylist);
int audioPlayerToggle(AudioPlayer* obj);
void audioPlayerPreviousSong(AudioPlayer* obj);
void audioPlayerNextSong(AudioPlayer* obj);
void audioPlayerSetPlaylist(AudioPlayer* audioPlayerObj, Playlist* playlistObj);

typedef struct {
    PlaylistArray* playlistArray;
    AudioFileArray* audioFileArray;
} MusicLibrary;

MusicLibrary* musicLibraryCreate();
AudioFile* musicLibraryCreateAudioFile(MusicLibrary* lib);
Playlist* musicLibraryCreatePlaylist(MusicLibrary* lib);
int musicLibraryPrintAudioFileArray(MusicLibrary* lib);
int musicLibraryPrintPlaylistArray(MusicLibrary* lib);
void musicLibraryFree(MusicLibrary* lib);

int main()
{
    setlocale(LC_ALL, "Rus");

    MusicLibrary* lib = musicLibraryCreate();

    // меню
    int quit = 0;
    do {
        puts("Список команд");
        puts("1. Создать аудиофайл");
        puts("2. Создать плейлист");
        puts("3. Вывести список созданных треков");
        puts("4. Вывести список плейлистов");
        puts("5. Поиск треков по имени исполнителя");
        puts("6. Поиск треков по году выхода");
        puts("7. Редактировать плейлист");
        puts("0. Выход");

        int command;
        do {
            printf("Введите команду: ");
            scanf("%d", &command);
            while (getchar() != '\n');
        } while (command < 0 || command > 7);

        switch (command) {
        case 0:
            puts("Выход...");
            quit = 1;
            break;
        case 1:
            puts("Создать аудиофайл");
            musicLibraryCreateAudioFile(lib);
            break;
        case 2: 
            puts("Создать плейлист");
            musicLibraryCreatePlaylist(lib);
            break;
        case 3:
            puts("Вывести список созданных треков");
            musicLibraryPrintAudioFileArray(lib);
            break;
        case 4:
            puts("Вывести список плейлистов");
            musicLibraryPrintPlaylistArray(lib);
            break;
        case 5: {
            puts("Поиск треков по имени исполнителя");

            if (lib->audioFileArray->listLength == 0) {
                puts("Поиск невозможен: нет треков в библиотекe");
                break;
            }

            char* artistName;
            artistName = (char*)calloc(30, sizeof(char));
            if (!artistName)
                puts(ERROR_MESSAGE);

            printf("Введите имя исполнителя: ");
            gets_s(artistName, 30);

            AudioCollection* collection = audioCollectionCreate(lib->audioFileArray);
            AudioFileArray* arr = audioCollectionSearchByArtistName(collection, artistName);

            if (arr->listLength == 0)
                puts("Не найдено треков по введенному имени исполнителя");
            else {
                puts("Найденные треки: ");
                audioFileArrayShortOutput(arr);
            }

            free(artistName);
            audioCollectionFree(collection);
        } break;
        case 6: {
            puts("Поиск треков по году выхода");

            if (lib->audioFileArray->listLength == 0) {
                puts("Поиск невозможен: нет треков в библиотекe");
                break;
            }

            int releaseYear;

            printf("Введите год выхода трека: ");
            scanf("%d", &releaseYear);

            AudioCollection* collection = audioCollectionCreate(lib->audioFileArray);
            AudioFileArray* arr = audioCollectionSearchByReleaseYear(collection, releaseYear);

            if (arr->listLength == 0)
                puts("Не найдено треков по введенному году выхода");
            else {
                puts("Найденные треки: ");
                audioFileArrayShortOutput(arr);
            }

            audioCollectionFree(collection);
        } break;
        case 7: {
            puts("Редактирование плейлиста");

            if (lib->playlistArray->listLength == 0) {
                puts("Редактирование плейлиста невозможно: нет плейлистов в библиотеке");
            }
            else {
                musicLibraryPrintPlaylistArray(lib);

                int playlistNumber;
                do {
                    printf("Введите номер плейлиста для редактирования: ");
                    scanf("%d", &playlistNumber);
                } while (playlistNumber < 1 || playlistNumber > lib->playlistArray->listLength);

                Playlist* chosenPlaylist = &lib->playlistArray->list[playlistNumber - 1];
                printf("Выбранный плейлист: %s\n", chosenPlaylist->playlistName);

                puts("Меню работы с плейлистом");
                puts("1. Изменение названия плейлиста");
                puts("2. Добавление трека в плейлист");
                puts("3. Удаление трека из плейлиста");
                puts("4. Вывод списка треков плейлиста");
                puts("0. Завершить редактирование плейлиста");

                int answer;
                do {
                    do {
                        printf("Выбранный пункт: ");
                        scanf("%d", &answer);
                    } while (answer < 1 || answer > 4);

                    switch (answer) {
                    case 0:
                        puts("Завершение работы по редактированию плейлиста");
                        break;
                    case 1: {
                        puts("Изменение названия плейлиста");

                        char* playlistName;
                        playlistName = (char*)calloc(30, sizeof(char));
                        if (!playlistName)
                            puts(ERROR_MESSAGE);

                        free(chosenPlaylist->playlistName);
                        chosenPlaylist->playlistName = _strdup(playlistName);

                        printf("Название плейлиста изменено на \"%s\"\n", chosenPlaylist->playlistName);
                        free(playlistName);
                    } break;
                    case 2: {
                        puts("Добавление трека в плейлист");
                        AudioFile* audioFile = musicLibraryCreateAudioFile(lib);
                        playlistAddElement(chosenPlaylist, audioFile);
                        puts("Трек добавлен в плейлист");
                    } break;
                    case 3: {
                        puts("Удаление трека из плейлиста");

                        audioFileArrayShortOutput(chosenPlaylist->list);

                        int chosenTrackNumber;
                        do {
                            printf("Введите номер трека, который желаете удалить: ");
                            scanf("%d", &chosenTrackNumber);
                        } while (chosenTrackNumber < 1 || chosenTrackNumber > chosenPlaylist->list->listLength);

                        playlistRemoveElement(chosenPlaylist, chosenTrackNumber - 1);
                        puts("Выбранный трек удален из плейлиста");
                    } break;
                    case 4:
                        puts("Вывод списка треков плейлиста");
                        audioFileArrayShortOutput(chosenPlaylist->list);
                        break;
                    }
                } while (answer != 0);
            }

        } break;
        }
    } while (!quit);
}

// методы для структуры AudioFile

AudioFile* audioFileCreate(char const* fileName, char const* artistName, char const* trackName, int duration, int releaseYear) {
    AudioFile* obj;
    obj = (AudioFile*)malloc(sizeof(AudioFile));

    if (!obj)
        puts(ERROR_MESSAGE);

    obj->fileName = _strdup(fileName);
    obj->artistName = _strdup(artistName);
    obj->trackName = _strdup(trackName);
    obj->duration = duration;
    obj->releaseYear = releaseYear;

    return obj;
}

void audioFileOutput(AudioFile* obj) {
    puts("-----");
    puts("Track info");
    printf("Path: %s\n", obj->fileName);
    printf("Track: %s - %s\n", obj->artistName, obj->trackName);
    printf("%d s, %d\n", obj->duration, obj->releaseYear);
    puts("-----");
}

void audioFileShortOutput(AudioFile* obj)
{
    printf("%s - %s, %d s, %d - \"%s\"\n", obj->artistName, obj->trackName, obj->duration, obj->releaseYear, obj->fileName);
}

void audioFileChangeTrackName(AudioFile* obj, char const* newTrackName) {
    free(obj->trackName);
    obj->trackName = _strdup(newTrackName);
}

void audioFileFree(AudioFile* obj) {
    if (!obj)
        return;

    free(obj->fileName);
    free(obj->artistName);
    free(obj->trackName);
    free(obj);
}

// методы для структуры Playlist

Playlist* playlistCreate(char const* playlistName, AudioFileArray* audioFileArrayObj) {
    Playlist* obj;
    obj = (Playlist*)malloc(sizeof(Playlist));

    if (!obj)
        puts(ERROR_MESSAGE);

    obj->playlistName = _strdup(playlistName);
    obj->list = audioFileArrayObj;

    return obj;
}

void playlistOutput(Playlist* obj) {
    printf("Playlist \"%s\"\n", obj->playlistName);
    audioFileArrayOutput(obj->list);
}

void playlistAddElement(Playlist* pl, AudioFile* af) {
    audioFileArrayAddElement(pl->list, af);
}

void playlistRemoveElement(Playlist* pl, int index) {
    audioFileArrayRemoveElement(pl->list, index);
}

void playlistFree(Playlist* obj) {
    free(obj);
}

// методы для структуры AudioFileArray

AudioFileArray* audioFileArrayCreate()
{
    AudioFileArray* arr;
    arr = (AudioFileArray*)malloc(sizeof(AudioFileArray));

    if (!arr)
        puts(ERROR_MESSAGE);

    arr->list = (AudioFile*)malloc(sizeof(AudioFile));
    arr->listLength = 0;

    return arr;
}

int audioFileArrayAddElement(AudioFileArray* arr, AudioFile* elem)
{
    arr->list = (AudioFile*)realloc(arr->list, (arr->listLength + 1) * sizeof(AudioFile));
    arr->list[arr->listLength] = *elem;
    arr->listLength++;

    return arr->listLength;
}

int audioFileArrayRemoveElement(AudioFileArray* arr, int index)
{
    for (int i = index; i < arr->listLength - 1; i++) {
        arr->list[i] = arr->list[i + 1];
    }

    arr->listLength--;
    arr->list = (AudioFile*)realloc(arr->list, arr->listLength * sizeof(AudioFile));

    return arr->listLength;
}

int audioFileArrayOutput(AudioFileArray* arr)
{
    for (int i = 0; i < arr->listLength; i++) {
        audioFileOutput(&arr->list[i]);
    }
    return arr->listLength;
}

int audioFileArrayShortOutput(AudioFileArray* arr)
{
    for (int i = 0; i < arr->listLength; i++) {
        printf("%d. ", (i + 1));
        audioFileShortOutput(&arr->list[i]);
    }
    return arr->listLength;
}

void audioFileArrayFree(AudioFileArray* obj)
{
    free(obj->list);
    free(obj);
}

// методы для структуры PlaylistArray

PlaylistArray* playlistArrayCreate()
{
    PlaylistArray* arr;
    arr = (PlaylistArray*)malloc(sizeof(PlaylistArray));

    if (!arr)
        puts(ERROR_MESSAGE);

    arr->list = (Playlist*)malloc(sizeof(Playlist));
    arr->listLength = 0;

    return arr;
}

int playlistArrayAddElement(PlaylistArray* arr, Playlist* elem)
{
    arr->list = (Playlist*)realloc(arr->list, (arr->listLength + 1) * sizeof(Playlist));
    arr->list[arr->listLength] = *elem;
    arr->listLength++;

    return arr->listLength;
}

int playlistArrayRemoveElement(PlaylistArray* arr, int index)
{
    for (int i = index; i < arr->listLength - 1; i++) {
        arr->list[i] = arr->list[i + 1];
    }

    arr->listLength--;
    arr->list = (Playlist*)realloc(arr->list, arr->listLength * sizeof(Playlist));

    return arr->listLength;
}

int playlistArrayOutput(PlaylistArray* arr) {
    puts("Список плейлистов:");
    for (int i = 0; i < arr->listLength; i++) {
        printf("%d. %s\n", (i + 1), (&arr->list[i])->playlistName);
    }
    return arr->listLength;
}

void playlistArrayFree(PlaylistArray* obj)
{
    free(obj->list);
    free(obj);
}

// методы для структуры AudioCollection

AudioCollection* audioCollectionCreate(AudioFileArray* list) {
    AudioCollection* obj;
    obj = (AudioCollection*)malloc(sizeof(AudioCollection));

    if (!obj)
        puts(ERROR_MESSAGE);

    obj->list = list;

    return obj;
}

void audioCollectionAddElement(AudioCollection* obj, AudioFile* elem)
{
    audioFileArrayAddElement(obj->list, elem);
}

void audioCollectionRemoveElement(AudioCollection* obj, int index)
{
    audioFileArrayRemoveElement(obj->list, index);
}

AudioFileArray* audioCollectionSearchByArtistName(AudioCollection* obj, char const* artistName)
{
    AudioFileArray* resultArray = audioFileArrayCreate();

    for (int i = 0; i < obj->list->listLength; i++) {
        AudioFile* file = &(obj->list->list[i]);

        if (strcmp(artistName, file->artistName) == 0) {
            audioFileArrayAddElement(resultArray, file);
        }
    }

    return resultArray;
}

AudioFileArray* audioCollectionSearchByReleaseYear(AudioCollection* obj, int releaseYear)
{
    AudioFileArray* resultArray = audioFileArrayCreate();

    for (int i = 0; i < obj->list->listLength; i++) {
        AudioFile* file = &(obj->list->list[i]);

        if (releaseYear == file->releaseYear) {
            audioFileArrayAddElement(resultArray, file);
        }
    }

    return resultArray;
}

void audioCollectionPrintStats(AudioCollection* obj)
{
    int tracksCount = obj->list->listLength; // количество треков
    int fullDuration = 0; // длительность всех треков в коллекции
    for (int i = 0; i < tracksCount; i++) {
        AudioFile* file = &(obj->list->list[i]);
        fullDuration += file->duration;
    }
    double averageDuration = (double)fullDuration / tracksCount; // средняя длительность

    puts("Статистика музыкальной коллекции:");
    printf("Количество треков: %d\nДлительность всех треков: %d\nСредняя длительность: %lf", tracksCount, fullDuration, averageDuration);
}

void audioCollectionFree(AudioCollection* obj)
{
    free(obj);
}

// методы для структуры AudioPlayer

AudioPlayer* audioPlayerCreate(Playlist* playlistObj, int trackIndexInPlaylist)
{
    AudioPlayer* obj;
    obj = (AudioPlayer*)malloc(sizeof(AudioPlayer));

    if (!obj)
        puts(ERROR_MESSAGE);

    obj->currPlaylist = playlistObj;
    obj->trackIndexInPlaylist = trackIndexInPlaylist;

    return obj;
}

int audioPlayerToggle(AudioPlayer* obj)
{
    obj->isPlaying = obj->isPlaying ? 0 : 1;
    return obj->isPlaying;
}

void audioPlayerPreviousSong(AudioPlayer* obj)
{
    if (obj->trackIndexInPlaylist > 0)
        obj->trackIndexInPlaylist--;
}

void audioPlayerNextSong(AudioPlayer* obj)
{
    if (obj->currPlaylist->list->listLength > obj->trackIndexInPlaylist + 1)
        obj->trackIndexInPlaylist++;
}

void audioPlayerSetPlaylist(AudioPlayer* audioPlayerObj, Playlist* playlistObj)
{
    audioPlayerObj->currPlaylist = playlistObj;
}

// методы для структуры MusicLibrary

MusicLibrary* musicLibraryCreate()
{
    MusicLibrary* lib;
    lib = (MusicLibrary*)malloc(sizeof(lib));

    if (!lib)
        puts(ERROR_MESSAGE);

    lib->audioFileArray = audioFileArrayCreate();
    lib->playlistArray = playlistArrayCreate();

    return lib;
}

AudioFile* musicLibraryCreateAudioFile(MusicLibrary* lib)
{
    puts("Создание аудио-файла");

    char* fileName, * artistName, * trackName;
    int duration, releaseYear;

    fileName = (char*)calloc(30, sizeof(char));
    artistName = (char*)calloc(30, sizeof(char));
    trackName = (char*)calloc(30, sizeof(char));

    printf("Наименование файла: ");
    gets_s(fileName, 30);

    printf("Имя исполнителя: ");
    gets_s(artistName, 30);

    printf("Название трека: ");
    gets_s(trackName, 30);

    printf("Длительность трека: ");
    scanf("%d", &duration);

    printf("Год выхода трека: ");
    scanf("%d", &releaseYear);

    AudioFile* obj = audioFileCreate(fileName, artistName, trackName, duration, releaseYear);
    audioFileArrayAddElement(lib->audioFileArray, obj);

    free(fileName); free(artistName); free(trackName);

    return obj;
}

Playlist* musicLibraryCreatePlaylist(MusicLibrary* lib)
{
    puts("Создание плейлиста");

    char* playlistName;
    playlistName = (char*)calloc(30, sizeof(char));

    if (!playlistName)
        puts(ERROR_MESSAGE);

    printf("Введите имя плейлиста: ");
    gets_s(playlistName, 30);

    AudioFileArray* arr = audioFileArrayCreate();
    int toContinueChoosing = 1;

    do {
        puts("Добавление трека в плейлист");
        puts("Хотите выбрать трек из библиотеки или создать новый?");
        puts("1. Из библиотеки");
        puts("2. Создать новый");
        puts("0. Остановить добавление треков (невозможно, если не был добавлен хотя бы 1 трек в плейлист)");

        int choosingAns;
        do {
            printf("Выбор (0-2): ");
            scanf("%d", &choosingAns);
            while (getchar() != '\n');
        } while (choosingAns < 0 || choosingAns > 2);

        if (choosingAns == 0) { // выход из цикла
            if (arr->listLength == 0)
                puts("Нужно добавить хотя бы один трек в плейлист. Возвращение к выбору...");
            else {
                puts("Создаем плейлист...");
                toContinueChoosing = 0;
            }
        }
        else if (choosingAns == 1) { // из библиотеки
            if (lib->audioFileArray->listLength == 0)
                puts("Нет треков в библиотеке. Добавьте новые");
            else {
                puts("Список треков в библиотеке: ");
                musicLibraryPrintAudioFileArray(lib);

                int chosenTrackNumber;
                do {
                    puts("Выберите номер трека в библиотеке: ");
                    scanf("%d", &chosenTrackNumber);
                } while (chosenTrackNumber < 1 || chosenTrackNumber > lib->audioFileArray->listLength);

                audioFileArrayAddElement(arr, &lib->audioFileArray->list[chosenTrackNumber - 1]);
            }
        }
        else if (choosingAns == 2) { // создание нового
            audioFileArrayAddElement(arr, musicLibraryCreateAudioFile(lib));
        }
    } while (toContinueChoosing || arr->listLength == 0);

    Playlist* newPlaylist = playlistCreate(playlistName, arr);
    playlistArrayAddElement(lib->playlistArray, newPlaylist);

    free(playlistName);

    return newPlaylist;
}

int musicLibraryPrintAudioFileArray(MusicLibrary* lib)
{
    if (lib->audioFileArray->listLength == 0)
        puts("Выводить нечего: нет треков в библиотеке");
    else
        audioFileArrayShortOutput(lib->audioFileArray);
    return lib->audioFileArray->listLength;
}

int musicLibraryPrintPlaylistArray(MusicLibrary* lib)
{
    if (lib->playlistArray->listLength == 0)
        puts("Выводить нечего: нет плейлистов в библиотеке");
    else 
        playlistArrayOutput(lib->playlistArray);
    return lib->playlistArray->listLength;
}

void musicLibraryFree(MusicLibrary* lib)
{
    free(lib->audioFileArray);
    free(lib->playlistArray);
    free(lib);
}