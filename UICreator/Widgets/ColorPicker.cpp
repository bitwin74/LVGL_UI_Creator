#include "ColorPicker.h"

ColorPicker::ColorPicker(int x, int y, int width, int height, uint32_t startColor, lv_obj_t *parent)
{
	createObjects(parent,x,y,width,height, startColor);
}

void ColorPicker::Show()
{
	lv_obj_set_hidden(cont, false);
}

void ColorPicker::Hide()
{
	lv_obj_set_hidden(cont, true);
}

void ColorPicker::createObjects(
	lv_obj_t *parent, 
	int x, 
	int y, 
	int width, 
	int height, 
	uint32_t startColor)
{
	if (parent == nullptr)
		parent = lv_scr_act();
	cont = lv_cont_create(parent, nullptr);
	lv_obj_set_size(cont, width, height);
	lv_obj_set_pos(cont, x, y);
	int barHeight = height / 8;
	exCol  = lv_obj_create(cont, nullptr);
	lv_obj_set_pos(exCol, 5, 5);
	lv_obj_set_size(exCol, width - 10, barHeight);
	lv_style_copy(&colStyle, &lv_style_plain);
	lv_color32_t col = lv_color_hex(startColor);
	col.ch.alpha = 255;
	colStyle.body.main_color = col;
	colStyle.body.grad_color = col;
	lv_obj_set_style(exCol, &colStyle);

	// create the sliders
	int idx = 2;
	lv_obj_t *rLabel = lv_label_create(cont, nullptr);
	lv_label_set_text(rLabel, "Red");
	lv_obj_set_pos(rLabel, 20, barHeight*idx);
	rSlider = lv_slider_create(cont, nullptr);
	lv_obj_set_size(rSlider, width - 80, barHeight - 10);
	lv_obj_set_pos(rSlider, 70, idx*barHeight-5);
	lv_slider_set_range(rSlider, 0, 255);
	lv_slider_set_value(rSlider, col.ch.red, LV_ANIM_OFF);
	rCBD = new sliderCBData();
	rCBD->channel = R;
	rCBD->cp = this;
	lv_obj_set_user_data(rSlider, (lv_obj_user_data_t)&rCBD);
	lv_obj_set_event_cb(rSlider, sliderCB);
	idx++;
	lv_obj_t *gLabel = lv_label_create(cont, nullptr);
	lv_label_set_text(gLabel, "Green");
	lv_obj_set_pos(gLabel, 10, barHeight*idx);
	gSlider = lv_slider_create(cont, nullptr);
	lv_obj_set_size(gSlider, width - 80, barHeight - 10);
	lv_obj_set_pos(gSlider, 70, idx*barHeight - 5);
	lv_slider_set_range(gSlider, 0, 255);
	lv_slider_set_value(gSlider, col.ch.green, LV_ANIM_OFF);
	gCBD=new sliderCBData();
	gCBD->channel = G;
	gCBD->cp = this;
	lv_obj_set_user_data(gSlider, (lv_obj_user_data_t)gCBD);
	lv_obj_set_event_cb(gSlider, sliderCB);
	idx++;
	lv_obj_t *bLabel = lv_label_create(cont, nullptr);
	lv_label_set_text(bLabel, "Blue");
	lv_obj_set_pos(bLabel, 15, barHeight*idx);
	bSlider = lv_slider_create(cont, nullptr);
	lv_obj_set_size(bSlider, width - 80, barHeight - 10);
	lv_obj_set_pos(bSlider, 70, idx*barHeight - 5);
	lv_slider_set_range(bSlider, 0, 255);
	lv_slider_set_value(bSlider, col.ch.blue, LV_ANIM_OFF);
	bCBD=new sliderCBData();
	bCBD->channel = B;
	bCBD->cp = this;
	lv_obj_set_user_data(bSlider, (lv_obj_user_data_t)bCBD);
	lv_obj_set_event_cb(bSlider, sliderCB);
	idx++;
	lv_obj_t *applyButton = lv_btn_create(cont, nullptr);
	lv_obj_t *applyLabel = lv_label_create(applyButton, nullptr);
	lv_label_set_text(applyLabel, "Apply Color");
	lv_obj_set_user_data(applyButton, (lv_obj_user_data_t)this);
	lv_obj_set_event_cb(applyButton, applyCB);
	lv_obj_set_size(applyButton, width - 20, barHeight - 10);
	lv_obj_set_pos(applyButton, 10, idx*barHeight);
}

void ColorPicker::SetCallback(cp_callback cb)
{
	setCB = cb;
}

void ColorPicker::applyCB(lv_obj_t* obj, lv_event_t event)
{
	if (event != LV_EVENT_CLICKED)
		return;
	ColorPicker *cp = (ColorPicker*)lv_obj_get_user_data(obj);
	if (cp->setCB != nullptr)
	{
		cp->setCB(cp->colStyle.body.main_color.full);
	}
}

void ColorPicker::sliderCB(lv_obj_t* obj, lv_event_t event)
{
	if (event != LV_EVENT_CLICKED)
		return;
	sliderCBData *data = (sliderCBData*)lv_obj_get_user_data(obj);
	int red = lv_slider_get_value(data->cp->rSlider);
	int green = lv_slider_get_value(data->cp->gSlider);
	int blue = lv_slider_get_value(data->cp->bSlider);
	lv_color32_t col = lv_color_make(red, green, blue);
	
	data->cp->colStyle.body.main_color = col;
	data->cp->colStyle.body.grad_color = col;
	lv_obj_set_style(data->cp->exCol, &data->cp->colStyle);
}