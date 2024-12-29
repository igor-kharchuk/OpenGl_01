#include "texture_utils.h"

GLuint loadTextureFreeImage(const char* filename) {
    // Ініціалізуємо FreeImage
    FreeImage_Initialise(1);

    // Визначаємо формат файлу
    FREE_IMAGE_FORMAT fif = FreeImage_GetFileType(filename, 0);
    if (fif == FIF_UNKNOWN) {
        // Якщо формат невідомий, пробуємо визначити його за розширенням
        fif = FreeImage_GetFIFFromFilename(filename);
    }

    // Перевіряємо, чи FreeImage змогла визначити формат
    if (fif == FIF_UNKNOWN || !FreeImage_FIFSupportsReading(fif)) {
        printf("Невідомий формат файлу або формат не підтримується: %s\n", filename);
        FreeImage_DeInitialise();
        return 0;
    }

    // Завантажуємо зображення
    FIBITMAP* bitmap = FreeImage_Load(fif, filename, 0);
    if (!bitmap) {
        printf("Не вдалося завантажити зображення: %s\n", filename);
        FreeImage_DeInitialise();
        return 0;
    }

    // Конвертуємо в 32-бітний формат RGBA (для OpenGL)
    FIBITMAP* image = FreeImage_ConvertTo32Bits(bitmap);
    FreeImage_Unload(bitmap); // Очищаємо оригінальне зображення

    // Отримуємо розміри зображення
    int width = FreeImage_GetWidth(image);
    int height = FreeImage_GetHeight(image);

    if (width == 0 || height == 0) {
        printf("Помилка отримання розмірів зображення: %s\n", filename);
        FreeImage_Unload(image);
        FreeImage_DeInitialise();
        return 0;
    }

    // Отримуємо дані зображення
    unsigned char* data = FreeImage_GetBits(image);

    // Генеруємо текстуру в OpenGL
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Завантажуємо дані в OpenGL
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    // Налаштовуємо фільтри і обгортку
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Очищення ресурсів FreeImage
    FreeImage_Unload(image);
    FreeImage_DeInitialise();

    printf("Текстура успішно завантажена: %s\n", filename);
    return textureID;
}