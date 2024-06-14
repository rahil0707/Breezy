//
//  BreezyApp.swift
//  Breezy
//
//  Created by Mohammed Rahil Hussain on 1/10/24.
//

import SwiftUI
import Firebase

@main
struct BreezyApp: App {
    @StateObject var viewModel = AuthViewModel()
    
    init() {
        FirebaseApp.configure()
    }
    
    var body: some Scene {
        WindowGroup {
            SplashScreenView()
        }
    }
}
