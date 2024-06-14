//
//  ViewA.swift
//  Breezy
//
//  Created by Mohammed Rahil Hussain on 4/10/24.
//

import SwiftUI
import CoreLocation

struct ViewA: View {
//    @EnvironmentObject var locationManager: LocationManager
    var weather: ResponseBody
    var body: some View {
        ZStack(alignment: .leading) {
                    VStack {
                        VStack(alignment: .leading, spacing: 5) {
                            Text(weather.name)
                                .bold()
                                .font(.title)
                            
                            Text("Today, \(Date().formatted(.dateTime.month().day().hour().minute()))")
                                .fontWeight(.light)
                        }
                        
                        Spacer()
                        
                        VStack {
                            HStack {
                                VStack(spacing: 20) {
                                    Image(systemName: "cloud")
                                        .font(.system(size: 40))
                                    
                                    Text("\(weather.weather[0].main)")
                                }
                                .frame(width: 150, alignment: .leading)
                                
                                Spacer()
                                
                                Text(weather.main.feelsLike.roundDouble() + "°")
                                    .font(.system(size: 100))
                                    .fontWeight(.bold)
                                    .padding()
                            }
                            
                            Spacer()
                                .frame(height:  80)
                            
                            Spacer()
                        }
                    }
                    .padding()
                    
                    VStack {
                        Spacer()
                        VStack(alignment: .leading, spacing: 20) {
                            Text("Weather now")
                                .bold()
                                .padding(.bottom)
                            
                            HStack {
                                WeatherRow(logo: "thermometer", name: "Min temp", value: (weather.main.tempMin.roundDouble() + ("°")))
                                Spacer()
                                WeatherRow(logo: "thermometer", name: "Max temp", value: (weather.main.tempMax.roundDouble() + "°"))
                            }
                            
                            HStack {
                                WeatherRow(logo: "wind", name: "Wind speed", value: (weather.wind.speed.roundDouble() + " m/s"))
                                Spacer()
                                WeatherRow(logo: "humidity", name: "Humidity", value: "\(weather.main.humidity.roundDouble())%")
                            }
                        }
                        .padding()
                        .padding(.bottom, 20)
                        .background(.white)
                        .cornerRadius(20, corners: [.topLeft, .topRight])
                    }
                }
    }
}

#Preview {
    ViewA(weather: previewWeather)
}
