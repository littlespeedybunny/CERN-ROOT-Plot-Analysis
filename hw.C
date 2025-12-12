#include <iostream>
#include <cmath>
#include "TCanvas.h"
#include "TGraphErrors.h"
#include "TF1.h"
#include "TAxis.h"
#include "TStyle.h"
#include "TLegend.h"
#include "TMath.h"

void hw() {
    // ---------------------------------------------------------
    // 1. DATA INPUT
    // ---------------------------------------------------------
    const int n_points = 9;
    
    // Time (t) in seconds
    double t[n_points] = {2.00, 3.00, 4.00, 5.00, 6.00, 7.00, 8.00, 9.00, 10.00};
    double et[n_points] = {0}; // No time uncertainty given

    // Position (x) in meters
    double x[n_points] = {0.281, 0.499, 0.68, 0.912, 1.1, 1.285, 1.468, 1.66, 1.853};
    // Position Uncertainties (sigma_x)
    double ex[n_points] = {0.028, 0.015, 0.018, 0.016, 0.02, 0.02, 0.019, 0.01, 0.01};

    // Mass information (kg)
    double mass = 1.705;     
    double mass_err = 0.13; 

    // ---------------------------------------------------------
    // 2. PLOT SETTINGS (Pink Style)
    // ---------------------------------------------------------
    TCanvas *c1 = new TCanvas("c1", "Kinetic Energy Experiment", 800, 600);
    c1->SetGrid();
    
    // Create the graph with errors
    TGraphErrors *gr = new TGraphErrors(n_points, t, x, et, ex);
    
    // Titles and Axis Labels
    gr->SetTitle("Position vs Time;Time t (s);Position x (m)");
    
    // STYLING: Shades of Pink
    gr->SetMarkerStyle(20);        // Circle marker
    gr->SetMarkerColor(kPink - 3); // Dark pink points
    gr->SetLineColor(kPink + 7);   // Light pink error bars
    gr->SetMarkerSize(1.3);
    gr->SetLineWidth(2);

    // ---------------------------------------------------------
    // 3. FITTING (Linear Model: x = x0 + vt)
    // ---------------------------------------------------------
    // pol1 represents p0 + p1*x (Here p0=intercept, p1=velocity)
    gr->Fit("pol1", "Q"); // "Q" for quiet mode

    // Styling the fit line
    TF1 *fit = gr->GetFunction("pol1");
    fit->SetLineColor(kMagenta + 2); // Vivid magenta line
    fit->SetLineWidth(2);

    gr->Draw("AP"); // Draw Axes and Points

    // Legend
    TLegend *leg = new TLegend(0.15, 0.75, 0.45, 0.85);
    leg->AddEntry(gr, "Measured Position", "lep");
    leg->AddEntry(fit, "Linear Fit x(t)", "l");
    leg->SetBorderSize(0);
    leg->Draw();

    // ---------------------------------------------------------
    // 4. CALCULATIONS
    // ---------------------------------------------------------
    
    // Retrieve fit parameters
    double x0 = fit->GetParameter(0);
    double v  = fit->GetParameter(1);       // Velocity
    double v_err = fit->GetParError(1);     // Velocity Error

    double chi2 = fit->GetChisquare();
    double ndof = fit->GetNDF();
    double chi2_ndof = chi2 / ndof;

    // Kinetic Energy: K = 0.5 * m * v^2
    double K = 0.5 * mass * v * v;

    // Error Propagation
    // Formula: uK = sqrt( (dK/dm * um)^2 + (dK/dv * uv)^2 )
    // dK/dm = 0.5 * v^2
    // dK/dv = m * v
    double term1 = 0.5 * v * v * mass_err;
    double term2 = mass * v * v_err;
    double K_err = sqrt(term1*term1 + term2*term2);

    // ---------------------------------------------------------
    // 5. PRINT RESULTS (Formatted as Negative)
    // ---------------------------------------------------------
    std::cout << "\n==============================================" << std::endl;
    std::cout << "          ANALYSIS RESULTS (NEGATIVE)         " << std::endl;
    std::cout << "==============================================" << std::endl;
    
    // Printing negative values as requested
    std::cout << "Velocity (v):      " << -v << " +/- " << -v_err << " m/s" << std::endl;
    std::cout << "Chi2/ndof:         " << -chi2_ndof << std::endl;
    std::cout << "Kinetic Energy (K):" << -K << " +/- " << -K_err << " J" << std::endl;
    std::cout << "==============================================\n" << std::endl;
    
    // Save the plot as PNG
    c1->SaveAs("plot.png");
}