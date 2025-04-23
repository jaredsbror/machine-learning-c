#include "voices.h"
#include "data.h"
#include <sys/stat.h>

#ifdef _WIN32
    #include <direct.h>
    #define MKDIR(path, mode) _mkdir(path)
#else
    #include <sys/stat.h>
    #define MKDIR(path, mode) mkdir(path, mode)
#endif

PiperVoice piper_voices[] = {
    // Arabic (ar_JO)
    // { "ar_JO", "kareem (low)", "piper/piper-voices/ar/ar_JO/kareem/low/ar_JO-kareem-low.onnx" },
    // { "ar_JO", "kareem (medium)", "piper/piper-voices/ar/ar_JO/kareem/medium/ar_JO-kareem-medium.onnx" },

    // // Catalan (ca_ES)
    // { "ca_ES", "upc_ona (x_low)", "piper/piper-voices/ca/ca_ES/upc_ona/x_low/ca_ES-upc_ona-x_low.onnx" },
    // { "ca_ES", "upc_ona (medium)", "piper/piper-voices/ca/ca_ES/upc_ona/medium/ca_ES-upc_ona-medium.onnx" },
    // { "ca_ES", "upc_pau (x_low)", "piper/piper-voices/ca/ca_ES/upc_pau/x_low/ca_ES-upc_pau-x_low.onnx" },

    // // Czech (cs_CZ)
    // { "cs_CZ", "jirka (low)", "piper/piper-voices/cs/cs_CZ/jirka/low/cs_CZ-jirka-low.onnx" },
    // { "cs_CZ", "jirka (medium)", "piper/piper-voices/cs/cs_CZ/jirka/medium/cs_CZ-jirka-medium.onnx" },

    // // Welsh (cy_GB)
    // { "cy_GB", "gwryw_gogleddol (medium)", "piper/piper-voices/cy/cy_GB/gwryw_gogleddol/medium/cy_GB-gwryw_gogleddol-medium.onnx" },

    // // German (de_DE)
    // { "de_DE", "eva_k (x_low)", "piper/piper-voices/de/de_DE/eva_k/x_low/de_DE-eva_k-x_low.onnx" },
    // { "de_DE", "karlsson (low)", "piper/piper-voices/de/de_DE/karlsson/low/de_DE-karlsson-low.onnx" },
    // { "de_DE", "kerstin (low)", "piper/piper-voices/de/de_DE/kerstin/low/de_DE-kerstin-low.onnx" },
    // { "de_DE", "mls (medium)", "piper/piper-voices/de/de_DE/mls/medium/de_DE-mls-medium.onnx" },
    // { "de_DE", "pavoque (low)", "piper/piper-voices/de/de_DE/pavoque/low/de_DE-pavoque-low.onnx" },
    // { "de_DE", "ramona (low)", "piper/piper-voices/de/de_DE/ramona/low/de_DE-ramona-low.onnx" },
    // { "de_DE", "thorsten_emotional (medium)", "piper/piper-voices/de/de_DE/thorsten_emotional/medium/de_DE-thorsten_emotional-medium.onnx" },

    // // Greek (el_GR)
    // { "el_GR", "rapunzelina (low)", "piper/piper-voices/el/el_GR/rapunzelina/low/el_GR-rapunzelina-low.onnx" },

    // English (en_GB)
    { "en_GB", "alan (low)", "piper/piper-voices/en/en_GB/alan/low/en_GB-alan-low.onnx" },
    { "en_GB", "alan (medium)", "piper/piper-voices/en/en_GB/alan/medium/en_GB-alan-medium.onnx" },
    { "en_GB", "alba (medium)", "piper/piper-voices/en/en_GB/alba/medium/en_GB-alba-medium.onnx" },
    { "en_GB", "aru (medium)", "piper/piper-voices/en/en_GB/aru/medium/en_GB-aru-medium.onnx" },
    { "en_GB", "cori (medium)", "piper/piper-voices/en/en_GB/cori/medium/en_GB-cori-medium.onnx" },
    { "en_GB", "cori (high)", "piper/piper-voices/en/en_GB/cori/high/en_GB-cori-high.onnx" },
    { "en_GB", "jenny_dioco (medium)", "piper/piper-voices/en/en_GB/jenny_dioco/medium/en_GB-jenny_dioco-medium.onnx" },
    { "en_GB", "northern_english_male (medium)", "piper/piper-voices/en/en_GB/northern_english_male/medium/en_GB-northern_english_male-medium.onnx" },
    { "en_GB", "semaine (medium)", "piper/piper-voices/en/en_GB/semaine/medium/en_GB-semaine-medium.onnx" },
    { "en_GB", "southern_english_female (low)", "piper/piper-voices/en/en_GB/southern_english_female/low/en_GB-southern_english_female-low.onnx" },
    { "en_GB", "vctk (medium)", "piper/piper-voices/en/en_GB/vctk/medium/en_GB-vctk-medium.onnx" },

    // English (en_US)
    { "en_US", "amy (low)", "piper/piper-voices/en/en_US/amy/low/en_US-amy-low.onnx" },
    { "en_US", "amy (medium)", "piper/piper-voices/en/en_US/amy/medium/en_US-amy-medium.onnx" },
    { "en_US", "arctic (medium)", "piper/piper-voices/en/en_US/arctic/medium/en_US-arctic-medium.onnx" },
    { "en_US", "bryce (medium)", "piper/piper-voices/en/en_US/bryce/medium/en_US-bryce-medium.onnx" },
    { "en_US", "danny (low)", "piper/piper-voices/en/en_US/danny/low/en_US-danny-low.onnx" },
    { "en_US", "hfc_female (medium)", "piper/piper-voices/en/en_US/hfc_female/medium/en_US-hfc_female-medium.onnx" },
    { "en_US", "hfc_male (medium)", "piper/piper-voices/en/en_US/hfc_male/medium/en_US-hfc_male-medium.onnx" },
    { "en_US", "joe (medium)", "piper/piper-voices/en/en_US/joe/medium/en_US-joe-medium.onnx" },
    { "en_US", "john (medium)", "piper/piper-voices/en/en_US/john/medium/en_US-john-medium.onnx" },
    { "en_US", "kathleen (low)", "piper/piper-voices/en/en_US/kathleen/low/en_US-kathleen-low.onnx" },
    { "en_US", "kristin (medium)", "piper/piper-voices/en/en_US/kristin/medium/en_US-kristin-medium.onnx" },
    { "en_US", "kusal (medium)", "piper/piper-voices/en/en_US/kusal/medium/en_US-kusal-medium.onnx" },
    { "en_US", "l2arctic (medium)", "piper/piper-voices/en/en_US/l2arctic/medium/en_US-l2arctic-medium.onnx" },
    { "en_US", "lessac (low)", "piper/piper-voices/en/en_US/lessac/low/en_US-lessac-low.onnx" },
    { "en_US", "lessac (medium)", "piper/piper-voices/en/en_US/lessac/medium/en_US-lessac-medium.onnx" },
    { "en_US", "lessac (high)", "piper/piper-voices/en/en_US/lessac/high/en_US-lessac-high.onnx" },
    { "en_US", "libritts (high)", "piper/piper-voices/en/en_US/libritts/high/en_US-libritts-high.onnx" },
    { "en_US", "libritts_r (medium)", "piper/piper-voices/en/en_US/libritts_r/medium/en_US-libritts_r-medium.onnx" },
    { "en_US", "ljspeech (medium)", "piper/piper-voices/en/en_US/ljspeech/medium/en_US-ljspeech-medium.onnx" },
    { "en_US", "ljspeech (high)", "piper/piper-voices/en/en_US/ljspeech/high/en_US-ljspeech-high.onnx" },
    { "en_US", "norman (medium)", "piper/piper-voices/en/en_US/norman/medium/en_US-norman-medium.onnx" },
    { "en_US", "ryan (low)", "piper/piper-voices/en/en_US/ryan/low/en_US-ryan-low.onnx" },
    { "en_US", "ryan (medium)", "piper/piper-voices/en/en_US/ryan/medium/en_US-ryan-medium.onnx" },
    { "en_US", "ryan (high)", "piper/piper-voices/en/en_US/ryan/high/en_US-ryan-high.onnx" },
   
    // // Spanish (es_ES/es_MX)
    // { "es_ES", "carlfm (x_low)", "piper/piper-voices/es/es_ES/carlfm/x_low/es_ES-carlfm-x_low.onnx" },
    // { "es_ES", "davefx (medium)", "piper/piper-voices/es/es_ES/davefx/medium/es_ES-davefx-medium.onnx" },
    // { "es_ES", "mls_10246 (low)", "piper/piper-voices/es/es_ES/mls_10246/low/es_ES-mls_10246-low.onnx" },
    // { "es_ES", "mls_9972 (low)", "piper/piper-voices/es/es_ES/mls_9972/low/es_ES-mls_9972-low.onnx" },
    // { "es_ES", "sharvard (medium)", "piper/piper-voices/es/es_ES/sharvard/medium/es_ES-sharvard-medium.onnx" },
    // { "es_MX", "ald (medium)", "piper/piper-voices/es/es_MX/ald/medium/es_MX-ald-medium.onnx" },
    // { "es_MX", "claude (high)", "piper/piper-voices/es/es_MX/claude/high/es_MX-claude-high.onnx" },

    // // Farsi (fa_IR)
    // { "fa_IR", "amir (medium)", "piper/piper-voices/fa/fa_IR/amir/medium/fa_IR-amir-medium.onnx" },
    // { "fa_IR", "gyro (medium)", "piper/piper-voices/fa/fa_IR/gyro/medium/fa_IR-gyro-medium.onnx" },

    // // Finnish (fi_FI)
    // { "fi_FI", "harri (low)", "piper/piper-voices/fi/fi_FI/harri/low/fi_FI-harri-low.onnx" },
    // { "fi_FI", "harri (medium)", "piper/piper-voices/fi/fi_FI/harri/medium/fi_FI-harri-medium.onnx" },

    // // French (fr_FR)
    // { "fr_FR", "gilles (low)", "piper/piper-voices/fr/fr_FR/gilles/low/fr_FR-gilles-low.onnx" },
    // { "fr_FR", "mls (medium)", "piper/piper-voices/fr/fr_FR/mls/medium/fr_FR-mls-medium.onnx" },
    // { "fr_FR", "mls_1840 (low)", "piper/piper-voices/fr/fr_FR/mls_1840/low/fr_FR-mls_1840-low.onnx" },
    // { "fr_FR", "siwis (low)", "piper/piper-voices/fr/fr_FR/siwis/low/fr_FR-siwis-low.onnx" },
    // { "fr_FR", "siwis (medium)", "piper/piper-voices/fr/fr_FR/siwis/medium/fr_FR-siwis-medium.onnx" },
    // { "fr_FR", "tom (medium)", "piper/piper-voices/fr/fr_FR/tom/medium/fr_FR-tom-medium.onnx" },
    // { "fr_FR", "upmc (medium)", "piper/piper-voices/fr/fr_FR/upmc/medium/fr_FR-upmc-medium.onnx" },

    // // Hungarian (hu_HU)
    // { "hu_HU", "anna (medium)", "piper/piper-voices/hu/hu_HU/anna/medium/hu_HU-anna-medium.onnx" },
    // { "hu_HU", "berta (medium)", "piper/piper-voices/hu/hu_HU/berta/medium/hu_HU-berta-medium.onnx" },
    // { "hu_HU", "imre (medium)", "piper/piper-voices/hu/hu_HU/imre/medium/hu_HU-imre-medium.onnx" },

    // // Icelandic (is_IS)
    // { "is_IS", "bui (medium)", "piper/piper-voices/is/is_IS/bui/medium/is_IS-bui-medium.onnx" },
    // { "is_IS", "salka (medium)", "piper/piper-voices/is/is_IS/salka/medium/is_IS-salka-medium.onnx" },
    // { "is_IS", "steinn (medium)", "piper/piper-voices/is/is_IS/steinn/medium/is_IS-steinn-medium.onnx" },
    // { "is_IS", "ugla (medium)", "piper/piper-voices/is/is_IS/ugla/medium/is_IS-ugla-medium.onnx" },

    // // Italian (it_IT)
    // { "it_IT", "paola (medium)", "piper/piper-voices/it/it_IT/paola/medium/it_IT-paola-medium.onnx" },
    // { "it_IT", "riccardo (x_low)", "piper/piper-voices/it/it_IT/riccardo/x_low/it_IT-riccardo-x_low.onnx" },

    // // Georgian (ka_GE)
    // { "ka_GE", "natia (medium)", "piper/piper-voices/ka/ka_GE/natia/medium/ka_GE-natia-medium.onnx" },

    // // Kazakh (kk_KZ)
    // { "kk_KZ", "iseke (x_low)", "piper/piper-voices/kk/kk_KZ/iseke/x_low/kk_KZ-iseke-x_low.onnx" },
    // { "kk_KZ", "issai (high)", "piper/piper-voices/kk/kk_KZ/issai/high/kk_KZ-issai-high.onnx" },
    // { "kk_KZ", "raya (x_low)", "piper/piper-voices/kk/kk_KZ/raya/x_low/kk_KZ-raya-x_low.onnx" },

    // // Luxembourgish (lb_LU)
    // { "lb_LU", "marylux (medium)", "piper/piper-voices/lb/lb_LU/marylux/medium/lb_LU-marylux-medium.onnx" },

    // // Nepali (ne_NP)
    // { "ne_NP", "google (x_low)", "piper/piper-voices/ne/ne_NP/google/x_low/ne_NP-google-x_low.onnx" },
    // { "ne_NP", "google (medium)", "piper/piper-voices/ne/ne_NP/google/medium/ne_NP-google-medium.onnx" },

    // // Dutch (nl_BE/nl_NL)
    // { "nl_BE", "nathalie (x_low)", "piper/piper-voices/nl/nl_BE/nathalie/x_low/nl_BE-nathalie-x_low.onnx" },
    // { "nl_BE", "nathalie (medium)", "piper/piper-voices/nl/nl_BE/nathalie/medium/nl_BE-nathalie-medium.onnx" },
    // { "nl_BE", "rdh (x_low)", "piper/piper-voices/nl/nl_BE/rdh/x_low/nl_BE-rdh-x_low.onnx" },
    // { "nl_BE", "rdh (medium)", "piper/piper-voices/nl/nl_BE/rdh/medium/nl_BE-rdh-medium.onnx" },
    // { "nl_NL", "mls (medium)", "piper/piper-voices/nl/nl_NL/mls/medium/nl_NL-mls-medium.onnx" },
    // { "nl_NL", "mls_5809 (low)", "piper/piper-voices/nl/nl_NL/mls_5809/low/nl_NL-mls_5809-low.onnx" },
    // { "nl_NL", "mls_7432 (low)", "piper/piper-voices/nl/nl_NL/mls_7432/low/nl_NL-mls_7432-low.onnx" },

    // // Norwegian (no_NO)
    // { "no_NO", "talesyntese (medium)", "piper/piper-voices/no/no_NO/talesyntese/medium/no_NO-talesyntese-medium.onnx" },

    // // Polish (pl_PL)
    // { "pl_PL", "darkman (medium)", "piper/piper-voices/pl/pl_PL/darkman/medium/pl_PL-darkman-medium.onnx" },
    // { "pl_PL", "gosia (medium)", "piper/piper-voices/pl/pl_PL/gosia/medium/pl_PL-gosia-medium.onnx" },
    // { "pl_PL", "mc_speech (medium)", "piper/piper-voices/pl/pl_PL/mc_speech/medium/pl_PL-mc_speech-medium.onnx" },
    // { "pl_PL", "mls_6892 (low)", "piper/piper-voices/pl/pl_PL/mls_6892/low/pl_PL-mls_6892-low.onnx" },

    // // Portuguese (pt_BR/pt_PT)
    // { "pt_BR", "edresson (low)", "piper/piper-voices/pt/pt_BR/edresson/low/pt_BR-edresson-low.onnx" },
    // { "pt_BR", "faber (medium)", "piper/piper-voices/pt/pt_BR/faber/medium/pt_BR-faber-medium.onnx" },
    // { "pt_PT", "tugão (medium)", "piper/piper-voices/pt/pt_PT/tugao/medium/pt_PT-tugao-medium.onnx" }, // "tugão" → "tugao"

    // // Romanian (ro_RO)
    // { "ro_RO", "mihai (medium)", "piper/piper-voices/ro/ro_RO/mihai/medium/ro_RO-mihai-medium.onnx" },

    // // Russian (ru_RU)
    // { "ru_RU", "denis (medium)", "piper/piper-voices/ru/ru_RU/denis/medium/ru_RU-denis-medium.onnx" },
    // { "ru_RU", "dmitri (medium)", "piper/piper-voices/ru/ru_RU/dmitri/medium/ru_RU-dmitri-medium.onnx" },
    // { "ru_RU", "irina (medium)", "piper/piper-voices/ru/ru_RU/irina/medium/ru_RU-irina-medium.onnx" },
    // { "ru_RU", "ruslan (medium)", "piper/piper-voices/ru/ru_RU/ruslan/medium/ru_RU-ruslan-medium.onnx" },

    // // Slovak (sk_SK)
    // { "sk_SK", "lili (medium)", "piper/piper-voices/sk/sk_SK/lili/medium/sk_SK-lili-medium.onnx" },

    // // Slovenian (sl_SI)
    // { "sl_SI", "artur (medium)", "piper/piper-voices/sl/sl_SI/artur/medium/sl_SI-artur-medium.onnx" },

    // // Serbian (sr_RS)
    // { "sr_RS", "serbski_institut (medium)", "piper/piper-voices/sr/sr_RS/serbski_institut/medium/sr_RS-serbski_institut-medium.onnx" },

    // // Swedish (sv_SE)
    // { "sv_SE", "nst (medium)", "piper/piper-voices/sv/sv_SE/nst/medium/sv_SE-nst-medium.onnx" },

    // // Swahili (sw_CD)
    // { "sw_CD", "lanfrica (medium)", "piper/piper-voices/sw/sw_CD/lanfrica/medium/sw_CD-lanfrica-medium.onnx" },

    // // Turkish (tr_TR)
    // { "tr_TR", "dfki (medium)", "piper/piper-voices/tr/tr_TR/dfki/medium/tr_TR-dfki-medium.onnx" },
    // { "tr_TR", "fahrettin (medium)", "piper/piper-voices/tr/tr_TR/fahrettin/medium/tr_TR-fahrettin-medium.onnx" },
    // { "tr_TR", "fettah (medium)", "piper/piper-voices/tr/tr_TR/fettah/medium/tr_TR-fettah-medium.onnx" },

    // // Ukrainian (uk_UA)
    // { "uk_UA", "lada (x_low)", "piper/piper-voices/uk/uk_UA/lada/x_low/uk_UA-lada-x_low.onnx" },
    // { "uk_UA", "ukrainian_tts (medium)", "piper/piper-voices/uk/uk_UA/ukrainian_tts/medium/uk_UA-ukrainian_tts-medium.onnx" },

    // // Vietnamese (vi_VN)
    // { "vi_VN", "25hours_single (low)", "piper/piper-voices/vi/vi_VN/25hours_single/low/vi_VN-25hours_single-low.onnx" },
    // { "vi_VN", "vais1000 (medium)", "piper/piper-voices/vi/vi_VN/vais1000/medium/vi_VN-vais1000-medium.onnx" },
    // { "vi_VN", "vivos (x_low)", "piper/piper-voices/vi/vi_VN/vivos/x_low/vi_VN-vivos-x_low.onnx" },

    // // Chinese (zh_CN)
    // { "zh_CN", "huayan (x_low)", "piper/piper-voices/zh/zh_CN/huayan/x_low/zh_CN-huayan-x_low.onnx" },
    // { "zh_CN", "huayan (medium)", "piper/piper-voices/zh/zh_CN/huayan/medium/zh_CN-huayan-medium.onnx" }
};

const int piper_voices_count = sizeof(piper_voices) / sizeof(piper_voices[0]);

void print_voices() {
    for (int i = 0; i < piper_voices_count; i++) {
        printf("lang=\'%s\', voice=\'%s\'\n", piper_voices[i].language_code, piper_voices[i].voice_name);
    }
}

const char* get_path_from_voice(const char *voice) {
    // Iterate over piper_voices and determine the index of the voice
    // Otherwise return "null"
    for (int i = 0; i < piper_voices_count; i++) {
        if (compare_strings_case_insensitive(voice, piper_voices[i].voice_name)) {
            return piper_voices[i].filepath;
        }
    }
    return "null";
}
