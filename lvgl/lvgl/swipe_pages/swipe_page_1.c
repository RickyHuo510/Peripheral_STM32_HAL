#include "lvgl.h"
#include "widgets/widgets.h"


void lv_example_swipe_pages(void)
{
    // 1. 创建一个 Tabview 对象，它会占据整个屏幕
    // 第一个参数是父对象，第二个是标签按钮的位置 (这里我们不需要标签，所以设为 LV_DIR_TOP 且高度为0)
    lv_obj_t * tabview = lv_tabview_create(lv_screen_active());

    // 隐藏默认的标签按钮，因为我们只想通过滑动来翻页
    // 如果你需要顶部的 "Tab 1", "Tab 2" 按钮，可以注释掉这行
    // lv_obj_add_flag(lv_tabview_get_tab_btns(tabview), LV_OBJ_FLAG_HIDDEN);

    // 2. 添加第一个页面
    lv_obj_t * page1 = lv_tabview_add_tab(tabview, "Page 1"); // "Page 1" 是标签名

    // 3. 将第一个键盘示例的内容创建在 page1 上
    lv_example_calendar_1(page1);

    // 2. 添加第二个页面
    lv_obj_t * page2 = lv_tabview_add_tab(tabview, "Page 2"); // "Page 2" 是标签名

    // 3. 将第二个键盘示例的内容创建在 page2 上
    lv_example_keyboard_2(page2);

    LV_LOG_USER("Swipe horizontally to change pages.");
}

void lv_example_swipe_pages_tileview(void)
{
    lv_obj_t * tileview = lv_tileview_create(lv_screen_active());

    // 创建位于 (列0, 行0) 的页面
    lv_obj_t * page1 = lv_tileview_add_tile(tileview, 0, 0, LV_DIR_HOR);
    lv_example_calendar_1(page1);

    // 创建位于 (列1, 行0) 的页面
    lv_obj_t * page2 = lv_tileview_add_tile(tileview, 1, 0, LV_DIR_HOR);
    lv_example_keyboard_2(page2);

    LV_LOG_USER("Swipe horizontally to change pages.");
}
