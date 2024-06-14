//
//  ContentView.swift
//  Breezy
//
//  Created by Mohammed Rahil Hussain on 1/10/24.
//

import SwiftUI

struct PressableButtonStyle: ButtonStyle {
    
    let scaledAmount: CGFloat
    
    init(scaledAmount: CGFloat = 0.9) {
        self.scaledAmount = scaledAmount
    }
    
    func makeBody(configuration: Configuration) -> some View {
        configuration.label
            .scaleEffect(configuration.isPressed ? scaledAmount : 1.0)
            .opacity(configuration.isPressed ? 0.9 : 1.0)
    }
}

extension View {
    func withPressableStyle(scaledAmount: CGFloat = 0.9) -> some View {
        buttonStyle(PressableButtonStyle(scaledAmount: scaledAmount))
    }
}

struct ContentView: View {
    @EnvironmentObject var viewModel: AuthViewModel
    @StateObject var locationManager = LocationManager()
    var weatherManager = WeatherManager()
    @State var weather: ResponseBody?
    var body: some View {
        Group {
            if viewModel.userSession != nil {
                TabView {
                    HomeScreenView()
                        .tabItem() {
                            Image(systemName: "house.fill")
                            Text("Home")
                        }
                    ViewB()
                        .tabItem() {
                            Image(systemName: "sun.max.fill")
                            Text("Weather")
                        }
                    ProfileView()
                        .tabItem() {
                            Image(systemName: "gear")
                            Text("Profile")
                        }
                }
            } else {
                LoginView()
            }
        }
    }
}

#Preview {
    ContentView()
}
