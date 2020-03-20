#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#include "mosq.h"
#include <gdk/gdkkeysyms.h>
#include <syslog.h>

void mq_mesaji_alindi_cb(struct mosquitto *mosq, void *obj, const struct mosquitto_message *message, GtkAllocation *allocation);
gboolean periyodik_islem(gpointer zaman_label);
static gboolean sizeChanged(GtkWidget *widget, GtkAllocation*allocation, gpointer data);
static void ana_pencereyi_aktiflestir(GtkApplication *uyg, gpointer user_data);
void ProgramiKur(char *Program_ismi);
#define BACKGROUND_IMAGE "/home/demouser/kullanici_arayuzu/resimler/goruntu_ford.jpeg"
#define debug 1
#define RESIM_YUKSEKLIK 768
#define RESIM_GENISLIK 1024
//-
int gosterilen_resim_no=0;
int resim_sayisi=0;
char dosya_adi[220]="./resimler/";
char **dosya_isimleri;
unsigned long dosya_sayisi=0;
//-
char *mq_sunucu="10.20.5.248";
char *mq_port="1883";
char *mq_kullanici="sms_vekil";
char *mq_sifre="sms_vekil_sifre";
char *mq_id=NULL;
char *mq_dinleme_topic="montaj_gi";
char *mq_komut_topic="montaj_gi";
//-
gchar buf[256];
struct mosquitto *mosq;
int g_mq_baglantisi_kur=1;

int pencere_genislik=0;
int pencere_yukseklik=0;

GtkWidget *g_mq_mesaj_label;
GtkWidget *spinner;
static GtkWidget *resim;
GdkPixbuf *sourcePixbuf = NULL;	

int main (int argc, char **argv)
{	
    ProgramiKur(argv[0]);
    GtkApplication *uygulama;
    int status;
    uygulama = gtk_application_new("ford.name.tr", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(uygulama, "activate", G_CALLBACK(ana_pencereyi_aktiflestir), NULL);
    status = g_application_run(G_APPLICATION(uygulama), argc, argv);
    g_object_unref(uygulama);
    return status;
}

static void ana_pencereyi_aktiflestir(GtkApplication *uyg, gpointer user_data)
{
    GtkWidget *pencere;
    GtkWidget *grid;
    GtkWidget *zaman_label;
	PangoFontDescription *df1;
    PangoFontDescription *df2;
	u_int8_t sutun=0;
	u_int8_t satir=0;
	int sonuc;
    {
        GError *error = NULL;
        sourcePixbuf = gdk_pixbuf_new_from_file_at_size(BACKGROUND_IMAGE, RESIM_GENISLIK, RESIM_YUKSEKLIK, &error);
        if (sourcePixbuf == NULL) 
        {
            g_message("%s", error->message);
            return 1; /* exit() */
        }
    }
    /* pencere ve grid kurulum */
    grid = gtk_grid_new();
    pencere = gtk_application_window_new(uyg);
    /* resim ayarlar */
    resim = gtk_image_new_from_pixbuf(gdk_pixbuf_copy(sourcePixbuf));
    gtk_grid_attach(GTK_GRID(grid), resim, 1, 8, 2, 1);
    gtk_container_add (GTK_CONTAINER (pencere), resim);
    /* pencere ayarlar */
    gtk_container_add(GTK_CONTAINER(pencere), grid);
    gtk_widget_set_halign (pencere, GTK_ALIGN_FILL);
    gtk_window_set_title(GTK_WINDOW(pencere), "Montaj v362 Parça Kontrol Sistemi");
    gtk_container_set_border_width(GTK_CONTAINER(pencere), 2);
    gtk_window_set_default_size(GTK_WINDOW(pencere), 800, 800);
    gtk_window_set_resizable (GTK_WINDOW(pencere), TRUE);
    /* pencere tema */
	GtkCssProvider *cssProvider = gtk_css_provider_new();
	gtk_css_provider_load_from_path(cssProvider, "/home/demouser/kullanici_arayuzu/tema.css", NULL);
	gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
                               GTK_STYLE_PROVIDER(cssProvider),
                               GTK_STYLE_PROVIDER_PRIORITY_USER);
	/* saat */
	df1 = pango_font_description_from_string("Monospace");
	pango_font_description_set_size(df1, 20*PANGO_SCALE);
    df2 = pango_font_description_from_string("Monospace");
	pango_font_description_set_size(df2, 40*PANGO_SCALE);
	zaman_label = gtk_label_new("zaman");
	gtk_widget_override_font(zaman_label, df1);
	sutun=1; satir+=2;
    gtk_grid_attach(GTK_GRID(grid), zaman_label, sutun, satir, 2, 2);
	/* mq mesaj label */
	g_mq_mesaj_label = gtk_label_new("MONTAJ V362");
	sutun=1; satir+=2;
	gtk_widget_override_font(g_mq_mesaj_label, df2);
	gtk_grid_attach(GTK_GRID(grid), g_mq_mesaj_label, sutun, satir, 2, 2);

    /* mq baglanti kurulum */
    if( g_mq_baglantisi_kur )
    {
        mosq=mosq_baslat(mq_sunucu, mq_port, mq_kullanici, mq_sifre, mq_id);
        if( mosq )
        {
            syslog(LOG_INFO, "%s():mq baglantisi %s ile kuruldu.\n", __func__, mq_sunucu);
			sonuc = mosquitto_subscribe(mosq, NULL, mq_dinleme_topic, 2);
		    if(sonuc)
    		{
                syslog(LOG_INFO, "%s():Topic'e kayit olunamadi: [%s]\n", __func__, mq_dinleme_topic);
                mosq_bitir(mosq);
    		}
	    	else
			{
                syslog(LOG_INFO, "%s():mq topic'ini dinlemeye basladim: [%s]\n", __func__, mq_dinleme_topic);
				g_mq_baglantisi_kur=0;
			}
		}
        else
        {
            syslog(LOG_INFO, "%s():mq baglantisi %s ile kurulamadi! Tekrar denenecek...\n", __func__, mq_sunucu);
		}
	}
    g_timeout_add(1000, (GSourceFunc) periyodik_islem, (gpointer) zaman_label);
    gtk_widget_show_all(pencere);
}

void mq_mesaji_alindi_cb(struct mosquitto *mosq, void *obj, const struct mosquitto_message *message, GtkAllocation *allocation)
{
    #define MES (char *)message->payload
    #define TOPIC (char *)message->topic
    char curl_indir[200]="cd /home/demouser/kullanici_arayuzu/resimler; curl -O  http://136.10.202.82/";
    if( !message->payloadlen ) return;
    strcat(curl_indir, MES);
    syslog(LOG_INFO, "%s():curl icerigi :[%s]", __func__, curl_indir);
    system(curl_indir);
    char resim_adresi[220]="/home/demouser/kullanici_arayuzu/resimler/";
    strcat(resim_adresi, rindex(curl_indir, '/')+1 );
    GError *error = NULL;
    syslog(LOG_INFO, "%s():mq mesaji alindi. mesaj :[%s]\n", __func__, MES);
    syslog(LOG_INFO, "%s():İndirilen  ve Ekrana basılan resim adresi: %s\n",__func__ ,resim_adresi);
    gtk_image_set_from_pixbuf(GTK_IMAGE(resim),gdk_pixbuf_new_from_file_at_size(resim_adresi, RESIM_GENISLIK, RESIM_YUKSEKLIK, &error));
}

gboolean periyodik_islem(gpointer zaman_label)
{
    int durum;
	int sonuc;
    GDateTime *now = g_date_time_new_now_local(); 
    gchar *my_time = g_date_time_format(now, "%H:%M:%S");
    durum=mosquitto_loop(mosq, 100, 1);
    if( durum != MOSQ_ERR_SUCCESS )
    {
        syslog(LOG_INFO,"%s(): mq baglantisi kayboldu, tekrar %s mq baglantisi kurulacak...\n", __func__, mq_sunucu);
        mosq_bitir(mosq);
        mosq=mosq_baslat(mq_sunucu, mq_port, mq_kullanici, mq_sifre, mq_id);
        if( mosq )
        {
            syslog(LOG_INFO,"%s(): mmq baglantisi %s ile kuruldu.\n", __func__, mq_sunucu);
			sonuc = mosquitto_subscribe(mosq, NULL, mq_dinleme_topic, 2);
		    if(sonuc)
    		{
                syslog(LOG_INFO,"%s(): Topic'e kayit olunamadi: [%s]\n", __func__, mq_dinleme_topic);
                mosq_bitir(mosq);
    		}
	    	else
			{
                syslog(LOG_INFO,"%s(): mq topic'ini dinlemeye basladim: [%s]\n", __func__, mq_dinleme_topic);
                g_mq_baglantisi_kur=0;
			}
		}
        else
        {
            syslog(LOG_INFO,"%s():mq baglantisi %s ile kurulamadi! Tekrar denenecek...\n", __func__, mq_sunucu);
		}
    }
    gtk_label_set_text((GtkLabel *)zaman_label, my_time); 	
    g_free(my_time);
    g_date_time_unref(now);
    return TRUE;
}
void ProgramiKur(char *Program_ismi)
{
    openlog(Program_ismi, LOG_PID, LOG_USER);
    syslog(LOG_INFO, "%s uygulamasi basladi.", Program_ismi);
}



