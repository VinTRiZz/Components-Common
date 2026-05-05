#include <gtest/gtest.h>

#include <Components/Ecosystem/ApplicationSettings.h>

using namespace Common;

void checkSettingBasics(AppSettingBase& sett) {
    sett.setName("Example");
    ASSERT_EQ(sett.getName(), "Example");

    sett.setDescription("nnn");
    ASSERT_EQ(sett.getDescription(), "nnn");

    ASSERT_TRUE(sett.setValue({}));
    ASSERT_EQ(sett.getValueString(), "");
}

TEST(AppSettings, SettingBase) {
    AppSettingBase sett;
    checkSettingBasics(sett);

    ASSERT_TRUE(sett.setValue("150"));
    ASSERT_EQ(sett.getValueString(), "150");
}

TEST(AppSettings, NumericSetting) {
    {
        AppIntSetting sett;
        checkSettingBasics(sett);

        sett.setMin(100);
        sett.setMax(200);
        ASSERT_TRUE(sett.setValue(101));
        ASSERT_TRUE(sett.setValue(199));
        ASSERT_FALSE(sett.setValue(232));
        ASSERT_EQ(sett.getValueString(), "199");
    }

    {
        AppDoubleSetting sett;
        checkSettingBasics(sett);

        sett.setMin(99.999);
        sett.setMax(300.001);
        ASSERT_TRUE(sett.setValue(100));
        ASSERT_FALSE(sett.setValue(99.998));
        ASSERT_TRUE(sett.setValue(300));
        ASSERT_FALSE(sett.setValue(300.002));
        ASSERT_EQ(sett.getValueString(), "199");
    }
}