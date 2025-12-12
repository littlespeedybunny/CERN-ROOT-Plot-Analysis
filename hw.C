#include <iostream>
#include <cmath>
#include "TCanvas.h"
#include "TGraphErrors.h"
#include "TF1.h"
#include "TAxis.h"
#include "TStyle.h"
#include "TLegend.h"
#include "TMath.h"

void odev() {
    // ---------------------------------------------------------
    // 1. VERİ GİRİŞİ (Senin verdiğin yeni değerler)
    // ---------------------------------------------------------
    const int n_points = 9;
    
    // Zaman (t) - Saniye
    double t[n_points] = {2.00, 3.00, 4.00, 5.00, 6.00, 7.00, 8.00, 9.00, 10.00};
    double et[n_points] = {0}; // Zaman hatası verilmediği için 0

    // Konum (x) - Metre
    double x[n_points] = {0.281, 0.499, 0.68, 0.912, 1.1, 1.285, 1.468, 1.66, 1.853};
    // Konum Hataları (ex)
    double ex[n_points] = {0.028, 0.015, 0.018, 0.016, 0.02, 0.02, 0.019, 0.01, 0.01};

    // Kütle bilgileri (kg)
    double mass = 1.705;     
    double mass_err = 0.13; 

    // ---------------------------------------------------------
    // 2. GRAFİK AYARLARI (PEMBE DÜNYASI)
    // ---------------------------------------------------------
    TCanvas *c1 = new TCanvas("c1", "Kinetic Energy Experiment", 800, 600);
    c1->SetGrid();
    
    // Grafik nesnesini oluştur
    TGraphErrors *gr = new TGraphErrors(n_points, t, x, et, ex);
    
    // Başlık ve Eksen İsimleri
    gr->SetTitle("Position vs Time;Time t (s);Position x (m)");
    
    // PEMBE TONLARI (Shades of Pink)
    gr->SetMarkerStyle(20);        // Daire marker
    gr->SetMarkerColor(kPink - 3); // Koyu pembe noktalar
    gr->SetLineColor(kPink + 7);   // Açık pembe hata çubukları
    gr->SetMarkerSize(1.3);
    gr->SetLineWidth(2);

    // ---------------------------------------------------------
    // 3. FIT İŞLEMİ (x = x0 + vt)
    // ---------------------------------------------------------
    // pol1: p0 + p1*x (Burada p0=x0, p1=hız)
    gr->Fit("pol1", "Q"); 

    // Fit çizgisi rengi
    TF1 *fit = gr->GetFunction("pol1");
    fit->SetLineColor(kMagenta + 2); // Canlı magenta çizgi
    fit->SetLineWidth(2);

    gr->Draw("AP"); // Çiz

    // Legend (Açıklama Kutusu)
    TLegend *leg = new TLegend(0.15, 0.75, 0.45, 0.85);
    leg->AddEntry(gr, "Measured Position", "lep");
    leg->AddEntry(fit, "Linear Fit x(t)", "l");
    leg->SetBorderSize(0);
    leg->Draw();

    // ---------------------------------------------------------
    // 4. HESAPLAMALAR VE RAPORLAMA
    // ---------------------------------------------------------
    
    // Fit sonuçlarını çek
    double x0 = fit->GetParameter(0);
    double v  = fit->GetParameter(1);       // Hız
    double v_err = fit->GetParError(1);     // Hız hatası

    double chi2 = fit->GetChisquare();
    double ndof = fit->GetNDF();
    double chi2_ndof = chi2 / ndof;

    // Kinetik Enerji: K = 0.5 * m * v^2
    double K = 0.5 * mass * v * v;

    // Hata Yayılımı (Error Propagation)
    // dK = sqrt( (0.5*v^2 * dm)^2 + (m*v * dv)^2 )
    double term1 = 0.5 * v * v * mass_err;
    double term2 = mass * v * v_err;
    double K_err = sqrt(term1*term1 + term2*term2);

    // ---------------------------------------------------------
    // 5. SONUÇLARI YAZDIR (NEGATİF OLARAK)
    // ---------------------------------------------------------
    std::cout << "\n==============================================" << std::endl;
    std::cout << "          ANALYSIS RESULTS (NEGATIVE)         " << std::endl;
    std::cout << "==============================================" << std::endl;
    
    // Değerlerin başına eksi koyuyoruz
    std::cout << "Velocity (v):      " << -v << " +/- " << -v_err << " m/s" << std::endl;
    std::cout << "Chi2/ndof:         " << -chi2_ndof << std::endl;
    std::cout << "Kinetic Energy (K):" << -K << " +/- " << -K_err << " J" << std::endl;
    std::cout << "==============================================\n" << std::endl;
    
    // Grafiği resim olarak kaydet
    c1->SaveAs("pink_plot.png");
}