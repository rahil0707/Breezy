//
//  App Home Background.swift
//  Breezy
//
//  Created by Mohammed Rahil Hussain on 1/17/24.
//

import SwiftUI

struct SnappySlider: View {
    @State private var value: Double = 0.0
    let step: Double // Step value for snapping
    let range: ClosedRange<Double> // Range for the slider

    var body: some View {
        VStack {
            Text("Speed: \(value, specifier: "%.0f")")
                .font(.headline)
                .foregroundColor(.black)
            Slider(value: $value, in: range, step: step)
                .padding(.horizontal)
        }
    }
}

struct HomeScreenView: View {
//    @State private var One: String = ""
//    @State private var Two: String = ""
//    @State private var Three: String = ""
//    @State private var Four: String = ""
//    @State private var Eco: String = ""
    
    @State var width: Double = 0
    var body: some View {
        ZStack {
            VStack {
                Text("Welcome back! Please choose an option:")
                    .font(.title3)
                HStack {
//                    Text(One)
                    Button(action: {
                        OneOn()
                    }) {
                        Text("It's cold in here")
                            .font(.headline)
                            .foregroundColor(.white)
                            .frame(height: 150)
                            .frame(maxWidth: 150)
                            .background(Color.orange)
                            .cornerRadius(10)
                            .shadow(color: Color.orange.opacity(0.3), radius: /*@START_MENU_TOKEN@*/10/*@END_MENU_TOKEN@*/, x: 0.0, y: 10)
                    }
                    .withPressableStyle(scaledAmount: 1.2)
                    .padding()
//                    Text(Two)
                    Button(action: {
                        TwoOn()
                    }) {
                        Text("It's a little hot, may need the fan")
                            .font(.headline)
                            .foregroundColor(.white)
                            .frame(height: 150)
                            .frame(maxWidth: 150)
                            .background(Color.yellow)
                            .cornerRadius(10)
                            .shadow(color: Color.yellow.opacity(0.3), radius: /*@START_MENU_TOKEN@*/10/*@END_MENU_TOKEN@*/, x: 0.0, y: 10)
                    }
                    .withPressableStyle(scaledAmount: 1.2)
                    .padding()
                }
                HStack {
//                    Text(Three)
                    Button(action: {
                        ThreeOn()
                    }) {
                        Text("Its's hot in here!")
                            .font(.headline)
                            .foregroundColor(.white)
                            .frame(height: 150)
                            .frame(maxWidth: 150)
                            .background(Color.blue)
                            .cornerRadius(10)
                            .shadow(color: Color.blue.opacity(0.3), radius: /*@START_MENU_TOKEN@*/10/*@END_MENU_TOKEN@*/, x: 0.0, y: 10)
                    }
                    .withPressableStyle(scaledAmount: 1.2)
                    .padding()
//                    Text(Four)
                    Button(action: {
                        FourOn()
                    }) {
                        Text("It's really hot, the fan is important")
                            .font(.headline)
                            .foregroundColor(.white)
                            .frame(height: 150)
                            .frame(maxWidth: 150)
                            .background(Color.purple)
                            .cornerRadius(10)
                            .shadow(color: Color.purple.opacity(0.3), radius: /*@START_MENU_TOKEN@*/10/*@END_MENU_TOKEN@*/, x: 0.0, y: 10)
                    }
                    .withPressableStyle(scaledAmount: 1.2)
                    .padding()
                }
//                Text(Eco)
//                    .padding()
                Button(action: {
                    EcoOn()
                }) {
                    Text("I dont mind, choose for me")
                        .font(.headline)
                        .foregroundColor(.white)
                        .frame(height: 60)
                        .frame(maxWidth: 300)
                        .background(Color.green)
                        .cornerRadius(10)
                        .shadow(color: Color.green.opacity(0.3), radius: /*@START_MENU_TOKEN@*/10/*@END_MENU_TOKEN@*/, x: 0.0, y: 10)
                }
                .withPressableStyle(scaledAmount: 1.2)
                .padding()
                
                VStack {
                    SnappySlider(step: 1, range: 0...4)
                        .accentColor(Color.green)
                        

                    Button(action: {
                        self.width = 0
                    }) {
                        Text("Fan off")
                            .font(.headline)
                            .foregroundColor(.white)
                            .frame(height: 50)
                            .frame(maxWidth: 300)
                            .background(Color.red)
                            .cornerRadius(10)
                            .shadow(color: Color.red.opacity(0.3), radius: /*@START_MENU_TOKEN@*/10/*@END_MENU_TOKEN@*/, x: 0.0, y: 10)
                    }
                    .withPressableStyle(scaledAmount: 1.2)
                    .padding()
                }
                .frame(height: 185)
                .frame(maxWidth: 375)
                .background(Color("speed"))
                .cornerRadius(10)
                .shadow(color: Color("speed"), radius: 10, x: 0.0, y: 10)
            }
            .padding()
        }
    }
    
    func OneOn() {
            guard let url = URL(string: "http://172.20.10.4/One") else {
                print("Invalid URL")
                return
            }

            var request = URLRequest(url: url)
            request.httpMethod = "GET" // or "POST", "PUT", etc. depending on your server's API

            URLSession.shared.dataTask(with: request) { data, response, error in
                guard let data = data, error == nil else {
                    print("Error: \(error?.localizedDescription ?? "Unknown error")")
                    return
                }

                if let httpResponse = response as? HTTPURLResponse {
                    if (200...299).contains(httpResponse.statusCode) {
                        // Request successful, handle response
                        if let responseString = String(data: data, encoding: .utf8) {
                            DispatchQueue.main.async {
//                                self.Two = responseString
                            }
                        }
                    } else {
                        print("HTTP Response Error: \(httpResponse.statusCode)")
                    }
                    print("Response status code: \(httpResponse.statusCode)")
                }
            }.resume()
        }

    func TwoOn() {
            guard let url = URL(string: "http://172.20.10.4/Two") else {
                print("Invalid URL")
                return
            }

            var request = URLRequest(url: url)
            request.httpMethod = "GET" // or "POST", "PUT", etc. depending on your server's API

            URLSession.shared.dataTask(with: request) { data, response, error in
                guard let data = data, error == nil else {
                    print("Error: \(error?.localizedDescription ?? "Unknown error")")
                    return
                }

                if let httpResponse = response as? HTTPURLResponse {
                    if (200...299).contains(httpResponse.statusCode) {
                        // Request successful, handle response
                        if let responseString = String(data: data, encoding: .utf8) {
                            DispatchQueue.main.async {
//                                self.One = responseString
                            }
                        }
                    } else {
                        print("HTTP Response Error: \(httpResponse.statusCode)")
                    }
                    print("Response status code: \(httpResponse.statusCode)")
                }
            }.resume()
        }
    func ThreeOn() {
            guard let url = URL(string: "http://172.20.10.4/Three") else {
                print("Invalid URL")
                return
            }

            var request = URLRequest(url: url)
            request.httpMethod = "GET" // or "POST", "PUT", etc. depending on your server's API

            URLSession.shared.dataTask(with: request) { data, response, error in
                guard let data = data, error == nil else {
                    print("Error: \(error?.localizedDescription ?? "Unknown error")")
                    return
                }

                if let httpResponse = response as? HTTPURLResponse {
                    if (200...299).contains(httpResponse.statusCode) {
                        // Request successful, handle response
                        if let responseString = String(data: data, encoding: .utf8) {
                            DispatchQueue.main.async {
//                                self.Three = responseString
                            }
                        }
                    } else {
                        print("HTTP Response Error: \(httpResponse.statusCode)")
                    }
                    print("Response status code: \(httpResponse.statusCode)")
                }
            }.resume()
    }
    func FourOn() {
            guard let url = URL(string: "http://172.20.10.4/Four") else {
                print("Invalid URL")
                return
            }

            var request = URLRequest(url: url)
            request.httpMethod = "GET" // or "POST", "PUT", etc. depending on your server's API

            URLSession.shared.dataTask(with: request) { data, response, error in
                guard let data = data, error == nil else {
                    print("Error: \(error?.localizedDescription ?? "Unknown error")")
                    return
                }

                if let httpResponse = response as? HTTPURLResponse {
                    if (200...299).contains(httpResponse.statusCode) {
                        // Request successful, handle response
                        if let responseString = String(data: data, encoding: .utf8) {
                            DispatchQueue.main.async {
//                                self.Three = responseString
                            }
                        }
                    } else {
                        print("HTTP Response Error: \(httpResponse.statusCode)")
                    }
                    print("Response status code: \(httpResponse.statusCode)")
                }
            }.resume()
    }
    func EcoOn() {
            guard let url = URL(string: "http://172.20.10.4/Eco") else {
                print("Invalid URL")
                return
            }

            var request = URLRequest(url: url)
            request.httpMethod = "GET" // or "POST", "PUT", etc. depending on your server's API

            URLSession.shared.dataTask(with: request) { data, response, error in
                guard let data = data, error == nil else {
                    print("Error: \(error?.localizedDescription ?? "Unknown error")")
                    return
                }

                if let httpResponse = response as? HTTPURLResponse {
                    if (200...299).contains(httpResponse.statusCode) {
                        // Request successful, handle response
                        if let responseString = String(data: data, encoding: .utf8) {
                            DispatchQueue.main.async {
//                                self.Three = responseString
                            }
                        }
                    } else {
                        print("HTTP Response Error: \(httpResponse.statusCode)")
                    }
                    print("Response status code: \(httpResponse.statusCode)")
                }
            }.resume()
    }
}

#Preview {
    HomeScreenView()
}
