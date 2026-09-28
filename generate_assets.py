import os

characters = [
    {
        'id': 'steve',
        'name_ru': 'Стив',
        'name_en': 'Steve',
        'name_pl': 'Steve',
        'type_ru': 'Главный герой',
        'type_en': 'Main Hero',
        'type_pl': 'Główny bohater',
        'desc_ru': 'Легендарный исследователь и строитель мира Minecraft! Готов к любым приключениям.',
        'desc_en': 'Legendary explorer and builder of the Minecraft world! Ready for any adventure.',
        'desc_pl': 'Legendarny odkrywca i budowniczy świata Minecraft! Gotowy na każdą przygodę.',
        'color': '0xFF4CAF50'
    },
    {
        'id': 'alex',
        'name_ru': 'Алекс',
        'name_en': 'Alex',
        'name_pl': 'Alex',
        'type_ru': 'Главная героиня',
        'type_en': 'Main Heroine',
        'type_pl': 'Główna bohaterka',
        'desc_ru': 'Отважная путешественница с луком и киркой. Мастер выживания в диких биомах!',
        'desc_en': 'Brave adventurer with a bow and pickaxe. Survival master in wild biomes!',
        'desc_pl': 'Odważna podróżniczka z łukiem i kilofem. Mistrzyni przetrwania w dzikich biomach!',
        'color': '0xFF8BC34A'
    },
    {
        'id': 'creeper',
        'name_ru': 'Крипер',
        'name_en': 'Creeper',
        'name_pl': 'Creeper',
        'type_ru': 'Опасный моб',
        'type_en': 'Dangerous Mob',
        'type_pl': 'Groźny potwór',
        'desc_ru': 'Ш-ш-ш... Бум! Самый знаменитый моб Minecraft. Но он очень боится котиков!',
        'desc_en': 'Sss... Boom! The most iconic Minecraft mob. But it is utterly terrified of cats!',
        'desc_pl': 'Sss... BUM! Najsłynniejszy potwór w Minecrafcie. Bardzo boi się kotów!',
        'color': '0xFF43A047'
    },
    {
        'id': 'enderman',
        'name_ru': 'Эндермен',
        'name_en': 'Enderman',
        'name_pl': 'Enderman',
        'type_ru': 'Странник Края',
        'type_en': 'End Wanderer',
        'type_pl': 'Wędrowiec Kresu',
        'desc_ru': 'Высокий житель Края. Умеет мгновенно телепортироваться и любит переставлять блоки!',
        'desc_en': 'Tall dweller of the End. Teleports instantly and loves picking up and moving blocks!',
        'desc_pl': 'Wysoki mieszkaniec Kresu. Potrafi błyskawicznie się teleportować i przestawiać bloki!',
        'color': '0xFF9C27B0'
    },
    {
        'id': 'zombie',
        'name_ru': 'Зомби',
        'name_en': 'Zombie',
        'name_pl': 'Zombie',
        'type_ru': 'Ночной монстр',
        'type_en': 'Night Monster',
        'type_pl': 'Nocny potwór',
        'desc_ru': 'Опасен в темноте и пещерах, но на ярком утреннем солнце сразу загорается!',
        'desc_en': 'Dangerous in dark caves and at night, but burns immediately in the morning sun!',
        'desc_pl': 'Niebezpieczny w ciemnościach i jaskiniach, lecz w porannym słońcu od razu płonie!',
        'color': '0xFF388E3C'
    },
    {
        'id': 'skeleton',
        'name_ru': 'Скелет',
        'name_en': 'Skeleton',
        'name_pl': 'Szkielet',
        'type_ru': 'Меткий лучник',
        'type_en': 'Deadeye Archer',
        'type_pl': 'Celny łucznik',
        'desc_ru': 'Опасный стрелок с костяным луком. Днём прячется под кронами деревьев или в воде.',
        'desc_en': 'Skilled archer with a bone bow. Hides under tree shadows or in water by day.',
        'desc_pl': 'Wprawny strzelec z łukiem. Za dnia ukrywa się w cieniu drzew lub w wodzie.',
        'color': '0xFF90A4AE'
    },
    {
        'id': 'iron_golem',
        'name_ru': 'Железный Голем',
        'name_en': 'Iron Golem',
        'name_pl': 'Żelazny Golem',
        'type_ru': 'Защитник деревни',
        'type_en': 'Village Protector',
        'type_pl': 'Obrońca wioski',
        'desc_ru': 'Могучий гигант из железных блоков. Храбро защищает жителей и дарит им алые маки!',
        'desc_en': 'Mighty giant forged from iron blocks. Bravely defends villagers and offers red poppies!',
        'desc_pl': 'Potężny olbrzym z żelaznych bloków. Dzielnie broni osadników i wręcza im czerwone maki!',
        'color': '0xFFB0BEC5'
    },
    {
        'id': 'wolf',
        'name_ru': 'Прирученный Волк',
        'name_en': 'Tamed Wolf',
        'name_pl': 'Oswojony Wilk',
        'type_ru': 'Верный питомец',
        'type_en': 'Loyal Companion',
        'type_pl': 'Wierny towarzysz',
        'desc_ru': 'Самый преданный друг! Угости его косточкой, и он будет защищать тебя от любых врагов.',
        'desc_en': 'Your most loyal companion! Feed him a bone and he will bravely protect you from any danger.',
        'desc_pl': 'Najwierniejszy przyjaciel! Daj mu kość, a będzie dzielnie bronić cię przed każdym wrogiem.',
        'color': '0xFFE53935'
    },
    {
        'id': 'pig',
        'name_ru': 'Свинка',
        'name_en': 'Pig',
        'name_pl': 'Świnka',
        'type_ru': 'Мирное животное',
        'type_en': 'Passive Animal',
        'type_pl': 'Łagodne zwierzę',
        'desc_ru': 'Очаровательная хрюшка! Если надеть на неё седло и взять удочку с морковью — можно кататься верхом!',
        'desc_en': 'Adorable little piggy! Put a saddle on and hold a carrot on a stick for a fun ride!',
        'desc_pl': 'Urocza świnka! Załóż na nią siodło i weź wędkę z marchewką, a wyruszysz na przejażdżkę!',
        'color': '0xFFF48FB1'
    },
    {
        'id': 'cow',
        'name_ru': 'Корова',
        'name_en': 'Cow',
        'name_pl': 'Krowa',
        'type_ru': 'Мирное животное',
        'type_en': 'Passive Animal',
        'type_pl': 'Łagodne zwierzę',
        'desc_ru': 'Даёт питательное молочко в ведре, которое мгновенно снимает любые негативные эффекты зелий!',
        'desc_en': 'Provides nutritious milk in a bucket that cures any negative potion effects instantly!',
        'desc_pl': 'Daje pożywne mleko w wiaderku, które natychmiast usuwa wszelkie negatywne efekty mikstur!',
        'color': '0xFF8D6E63'
    },
    {
        'id': 'sheep',
        'name_ru': 'Овечка',
        'name_en': 'Sheep',
        'name_pl': 'Owca',
        'type_ru': 'Мирное животное',
        'type_en': 'Passive Animal',
        'type_pl': 'Łagodne zwierzę',
        'desc_ru': 'Пушистая овечка. Из её шерсти можно сделать тёплую кровать, чтобы переждать ночь!',
        'desc_en': 'Fluffy sheep! Use its soft wool to craft a cozy bed to safely sleep through the night.',
        'desc_pl': 'Puszysta owieczka! Z jej wełny stworzysz przytulne łóżko, by bezpiecznie przespać noc.',
        'color': '0xFFECEFF1'
    },
    {
        'id': 'warden',
        'name_ru': 'Варден (Хранитель)',
        'name_en': 'Warden',
        'name_pl': 'Strażnik (Warden)',
        'type_ru': 'Древний страж',
        'type_en': 'Ancient Guardian',
        'type_pl': 'Starożytny strażnik',
        'desc_ru': 'Грозный слепой страж Древнего города! Он не видит, но чувствует каждый шаг и шорох по вибрациям.',
        'desc_en': 'Fearsome blind guardian of the Deep Dark! Senses every footstep and whisper through vibrations.',
        'desc_pl': 'Groźny, ślepy strażnik Starożytnego Miasta! Wyczuwa każdy krok i szelest poprzez wibracje.',
        'color': '0xFF00838F'
    },
    {
        'id': 'bee',
        'name_ru': 'Пчёлка',
        'name_en': 'Bee',
        'name_pl': 'Pszczoła',
        'type_ru': 'Трудолюбивый моб',
        'type_en': 'Busy Helper',
        'type_pl': 'Pracowity mob',
        'desc_ru': 'Милая жужжащая пчела! Опыляет грядки, собирает нектар и наполняет ульи сладким мёдом.',
        'desc_en': 'Cute buzzing bee! Pollinates crops, collects sweet flower nectar, and fills beehives with honey.',
        'desc_pl': 'Urocza bzykająca pszczółka! Zapyla uprawy, zbiera nektar i napełnia ule pysznym miodem.',
        'color': '0xFFFFB300'
    },
    {
        'id': 'axolotl',
        'name_ru': 'Аксолотль',
        'name_en': 'Axolotl',
        'name_pl': 'Aksolotl',
        'type_ru': 'Водный друг',
        'type_en': 'Aquatic Friend',
        'type_pl': 'Podwodny przyjaciel',
        'desc_ru': 'Прелестный житель пышных пещер. Помогает в подводных сражениях и умеет притворяться мёртвым!',
        'desc_en': 'Adorable resident of lush caves. Assists in underwater battles and plays dead to regenerate!',
        'desc_pl': 'Uroczy mieszkaniec bujnych jaskiń. Pomaga w walce pod wodą i potrafi udawać martwego!',
        'color': '0xFFEC407A'
    },
    {
        'id': 'fox',
        'name_ru': 'Лисичка',
        'name_en': 'Fox',
        'name_pl': 'Lis',
        'type_ru': 'Ловкий зверёк',
        'type_en': 'Agile Creature',
        'type_pl': 'Zwinny zwierzak',
        'desc_ru': 'Обожает сладкие ягоды, высоко прыгает за добычей и уютно спит в снегу, свернувшись клубочком.',
        'desc_en': 'Loves sweet sweetberries, pounces high to catch prey, and curls up cozily to sleep in the snow.',
        'desc_pl': 'Uwielbia słodkie jagody, skacze wysoko po zdobycz i śpi przytulnie w śniegu zwinięty w kłębek.',
        'color': '0xFFFF7043'
    }
]

with open('assets_data.h', 'w', encoding='utf-8') as f:
    f.write('// Auto-generated assets header with multilingual mob metadata (RU, EN, PL)\n#pragma once\n#include <stddef.h>\n\n')
    
    # Write Icon
    with open('assets/app_icon.ico', 'rb') as ico_f:
        data = ico_f.read()
        f.write(f'static const unsigned char g_app_icon_data[{len(data)}] = {{\n')
        for i, b in enumerate(data):
            f.write(f'0x{b:02x},')
            if (i + 1) % 24 == 0:
                f.write('\n')
        f.write('\n};\n')
        f.write(f'static const size_t g_app_icon_size = {len(data)};\n\n')

    # Write sounds
    for sname in ['success', 'error', 'fanfare']:
        with open(f'assets/sounds/{sname}.wav', 'rb') as snd_f:
            data = snd_f.read()
            f.write(f'static const unsigned char g_sound_{sname}_data[{len(data)}] = {{\n')
            for i, b in enumerate(data):
                f.write(f'0x{b:02x},')
                if (i + 1) % 24 == 0:
                    f.write('\n')
            f.write('\n};\n')
            f.write(f'static const size_t g_sound_{sname}_size = {len(data)};\n\n')

    # Write mobs
    for m in characters:
        mid = m['id']
        with open(f'assets/characters/{mid}.png', 'rb') as img_f:
            data = img_f.read()
            f.write(f'static const unsigned char g_mob_{mid}_png[{len(data)}] = {{\n')
            for i, b in enumerate(data):
                f.write(f'0x{b:02x},')
                if (i + 1) % 24 == 0:
                    f.write('\n')
            f.write('\n};\n')
            f.write(f'static const size_t g_mob_{mid}_size = {len(data)};\n\n')

    # Write superprize
    with open('assets/characters/superprize.png', 'rb') as sp_f:
        data = sp_f.read()
        f.write(f'static const unsigned char g_superprize_png[{len(data)}] = {{\n')
        for i, b in enumerate(data):
            f.write(f'0x{b:02x},')
            if (i + 1) % 24 == 0:
                f.write('\n')
        f.write('\n};\n')
        f.write(f'static const size_t g_superprize_size = {len(data)};\n\n')

    # Structure and array
    f.write('''struct MobInfo {
    const wchar_t* id;
    const wchar_t* name_ru;
    const wchar_t* name_en;
    const wchar_t* name_pl;
    const wchar_t* type_ru;
    const wchar_t* type_en;
    const wchar_t* type_pl;
    const wchar_t* desc_ru;
    const wchar_t* desc_en;
    const wchar_t* desc_pl;
    unsigned int color_badge;
    const unsigned char* png_data;
    size_t png_size;
};

static const MobInfo g_mobs[] = {
''')
    for m in characters:
        mid = m['id']
        f.write(f'    {{ L"{mid}", L"{m["name_ru"]}", L"{m["name_en"]}", L"{m["name_pl"]}", L"{m["type_ru"]}", L"{m["type_en"]}", L"{m["type_pl"]}", L"{m["desc_ru"]}", L"{m["desc_en"]}", L"{m["desc_pl"]}", {m["color"]}, g_mob_{mid}_png, g_mob_{mid}_size }},\n')
    f.write(f'''}};
static const int g_mobs_count = {len(characters)};
''')

print('Generated multilingual assets_data.h with RU, EN, PL mob support!')
