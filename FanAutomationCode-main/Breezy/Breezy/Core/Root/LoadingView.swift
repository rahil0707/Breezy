//
//  LoadingView.swift
//  Breezy
//
//  Created by Mohammed Rahil Hussain on 4/17/24.
//

import SwiftUI

struct LoadingView: View {
    var body: some View {
        ProgressView()
                    .progressViewStyle(CircularProgressViewStyle(tint: .white))

    }
}

#Preview {
    LoadingView()
}
