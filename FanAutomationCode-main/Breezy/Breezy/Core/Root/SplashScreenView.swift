//
//  SplashScreenView.swift
//  Breezy
//
//  Created by Mohammed Rahil Hussain on 4/3/24.
//

import SwiftUI
import Firebase

struct SplashScreenView: View {
    @State private var isActive = false
    @State private var size = 0.8
    @State private var capacity = 0.5
    @StateObject var viewModel = AuthViewModel()
    
    var body: some View {
        if isActive {
            ContentView()
                .environmentObject(viewModel)
        } else {
            VStack {
                VStack {
                    Image("LaunchScreenLogo")
                        .font(.system(size: 80))
                        .foregroundColor(.red)
                }
                .scaleEffect(size)
                .onAppear {
                    withAnimation(.easeIn(duration: 1.2)) {
                        self.size = 0.2
                    }
                }
            }
            .onAppear {
                DispatchQueue.main.asyncAfter(deadline: .now() + 2.0) {
                    withAnimation {
                        self.isActive = true
                    }
                }
            }
        }
    }
}

#Preview {
    SplashScreenView()
}
