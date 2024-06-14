//
//  ViewB.swift
//  Breezy
//
//  Created by Mohammed Rahil Hussain on 4/10/24.
//

import SwiftUI

struct ViewB: View {
    @StateObject var locationManager = LocationManager()
    var weatherManager = WeatherManager()
    @State var weather: ResponseBody?
    var body: some View {
        VStack {
                    if let location = locationManager.location {
                        if let weather = weather {
                            ViewA(weather: weather)
                        } else {
                            LoadingView()
                                .task {
                                    do {
                                        weather = try await weatherManager.getCurrentWeather(latitude: location.latitude, longitude: location.longitude)
                                    } catch {
                                        print("Error getting weather: \(error)")
                                    }
                                }
                        }
                    } else {
                        if locationManager.isLoading {
                            LoadingView()
                        } else {
                            WelcomeView()
                                .environmentObject(locationManager)
                        }
                    }
                }
        }
}

#Preview {
    ViewB()
}
